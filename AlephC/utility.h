#ifndef UTILITY_H
#define UTILITY_H

#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <shader.h>
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

struct Character {
    unsigned int TextureID;  
    glm::ivec2   Size;       
    glm::ivec2   Bearing;    
    unsigned int Advance;    
};

struct widgetProps {
    bool mouse_interactive;
    glm::vec4 padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 margin = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
    int colourLoc = 0;
    int index = 0;
};


class MousePicking
{
public:
    MousePicking() {}

    glm::vec3 normalize(double x_pos, double y_pos, int win_width, int win_height)
    {
        double nXPos = ((2 * x_pos)/win_width) - 1;
        double nYPos = 1 - ((2 * y_pos)/win_height);
        double nZPos;

        glReadPixels((int)x_pos, (int)y_pos, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &nZPos);

        return {nXPos, nYPos, 1.0f};
    }

    glm::vec3 unNormalize(float x_pos, float y_pos, int win_width, int win_height)
    {
        double uXPos = ((x_pos+1)*win_width)/2;
        double uYPos = ((y_pos-1)*win_height)/-2;
        return {uXPos, uYPos, 0.0f};
    }

    glm::vec3 getRay(glm::vec3 norm_win_coords ,glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, glm::vec4 viewport)
    {
        glm::mat4 model_view = view * model;
        glm::vec3 ray = glm::unProject(norm_win_coords, model_view, projection, viewport);
        return ray;
    }
};

class AudioSys
{
public:
    ma_engine engine;
    ma_result engineRes;
    
    AudioSys()
    {
        engineRes = ma_engine_init(nullptr, &engine);
        if (engineRes != MA_SUCCESS) std::cout << "Failed to init sound system\n";
        
    }

    void play_track(const std::vector<const char*> &track_list)
    {
        for (const char* path : track_list)
        {
            ma_engine_play_sound(&engine, path, nullptr);
        }
    }

    void play_single_sound(const char* soundPath)
    {
        ma_engine_play_sound(&engine, soundPath, nullptr);
    }
};

class TextRenderer
{
public:
    TextRenderer(const char* facePath,int size)
    {
        FT_Library lib;
        FT_Face face;
        if(FT_Init_FreeType(&lib))
        {
            std::cout << "An error occured during font initialization\n";
        }
        if(FT_New_Face(lib, facePath, 0, &face))
        {
            std::cout << "An error occured during font-face initialization\n";
        } else
        {
            FT_Set_Pixel_Sizes(face, 0,  size);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << "\n";
                    continue;
                }
                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // now store character for later use
                Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };
                Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(lib);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }


    void render_text(Shader &shader, std::string text, float x_pos, float y_pos, float scale, glm::vec3 colour)
    {
        shader.use();
        glUniform3f(glGetUniformLocation(shader.ID, "text_colour"), colour.x, colour.y, colour.z);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) 
        {
            Character ch = Characters[*c];

            float xpos = x_pos + ch.Bearing.x * scale;
            float ypos = y_pos - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },            
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }           
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x_pos += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
protected:
    std::map<GLchar, Character> Characters;
private:
    unsigned int VAO, VBO;
};

#endif