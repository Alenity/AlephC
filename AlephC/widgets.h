#ifndef WIDGET_H
#define WIDGET_H

#include <glad.h>
#include <glm/glm.hpp>
#include <shader.h>
#include <utility.h>
#include <string>
#include <utility>


struct uiVertexArr
{
    float vertices[12];
};

class Widget
{
public:
    Widget(glm::vec3 position, widgetProps props)
    {
        properties = props;
        pos = position;
        pos.x += properties.margin.x;
        pos.y -= properties.margin.y;
        width = 1 - (pos.x + properties.margin.z);
        height = 2 - (1 - pos.y + properties.margin.w);
        
        uiVertexArr vert;
        vert = genVertices();

        unsigned int indices[] = {
            0, 1, 3,
            0, 3, 2
        };
        

        glGenVertexArrays(1, &VAO);
        
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vert.vertices), vert.vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        
    }
    
    Widget(Widget &parent, widgetProps props)
    {
        properties = props;
        pos.x = parent.pos.x + parent.properties.padding.x + properties.margin.x;
        pos.y = parent.pos.y - parent.properties.padding.y - properties.margin.y;
        width = parent.width - (parent.properties.padding.x + parent.properties.padding.z + properties.margin.x + properties.margin.z);
        height = parent.height - (parent.properties.padding.y + parent.properties.padding.w + properties.margin.y + properties.margin.w);

        uiVertexArr vert = genVertices();

        unsigned int indices[] = {
            0, 1, 3,
            0, 3, 2
        };

        glGenVertexArrays(1, &VAO);
        
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vert.vertices), vert.vertices, GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        
    }

    uiVertexArr genVertices()
    {
        uiVertexArr vert;
        // create x coords
        for (int i = 0; i < 12; i += 3)
        {
            if(i%2 == 0) 
            {
                vert.vertices[i] = pos.x;
            }else
            {
                vert.vertices[i] = pos.x + width;
            }
        }
        // create y coords
        for (int i = 1; i < 12; i+=3)
        {
            if(i < 5)
            {
                vert.vertices[i] = pos.y;
                
            } else
            {
                vert.vertices[i] = pos.y - height;
            }
        }
        // create z coords
        for (int i = 2; i < 12; i+=3)
        {
            vert.vertices[i] = 0.0f;
        }

        return vert;
        
    }
protected:
    void draw(Shader &uiShader)
    {
        update_bounding_box();
        int colourLoc = glGetUniformLocation(uiShader.ID, "inputColour");
        uiShader.use();
        glUniform4f(colourLoc, properties.colour.x, properties.colour.y, properties.colour.z, properties.colour.w);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void update_bounding_box()
    {
        bounding_box[0] = pos.x;
        bounding_box[1] = pos.x + width;
        bounding_box[2] = pos.y;
        bounding_box[3] = pos.y - height;
    }

    void clean()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    unsigned int VAO, VBO, EBO;
public:
    float width;
    float height;
    glm::vec4 bounding_box;
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    widgetProps properties;
    
};

class Text: public Widget
{
public:
    Text(Widget &parent, const widgetProps& props, const std::string& content, glm::vec3 text_colour) : Widget(parent, props)
    {
        text = content;
        colour = text_colour;
        pos.x = parent.pos.x + parent.properties.margin.x + parent.properties.padding.x;
        pos.y = parent.pos.y - parent.properties.margin.y - parent.properties.padding.y;
    }

    void writeTo(Shader &uiShader, TextRenderer &renderer, Shader &textShader, float scale, float SCR_W, float SCR_H)
    {
        MousePicking picker;
        glm::vec3 position = picker.unNormalize(pos.x, pos.y, 1920, 1080);
        draw(uiShader);
        renderer.render_text(textShader, text, position.x, position.y, scale, colour);
    }

    void update(const char* content)
    {
        text = content;
    }
    void update(double value)
    {
        num_value = value;
    }
    void update(glm::vec3 col)
    {
        colour = col;
    }

    std::string text;
    double num_value;
    float font_size;
    glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Button: public Widget
{
    Button(Widget &parent, widgetProps props) : Widget(parent, props)
    {
        
    }
    
};

class Container: public Widget
{
    Container(glm::vec3 position, widgetProps props) : Widget(position, props)
    {
        
    }
};

class Slider: public Widget
{
    Slider(glm::vec3 position, widgetProps props, Text* link) : Widget(position, props)
    {
        linkedWidget = link;
    }

private:
    Text* linkedWidget;
    
};

#endif