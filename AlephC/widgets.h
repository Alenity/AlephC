#ifndef WIDGET_H
#define WIDGET_H

#include <glad.h>
#include <glm/glm.hpp>
#include <shader.h>
#include <vector>
#include <utility.h>
#include <string>
#include <memory>


struct uiVertexArr
{
    float vertices[12];
};

struct widgetProps {
    unsigned int ID;
    bool mouse_interactive;
    glm::vec4 padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 margin = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
};

class Widget
{
public:
    Widget(glm::vec3 position, const widgetProps& props, Shader *shader, Shader *pickShader) : shader(shader), pickShader(pickShader)
    {
        properties = props;
        pos = position;
        pos.x += properties.margin.x;
        pos.y -= properties.margin.y;
        width = 1 - (pos.x + properties.margin.z);
        height = 2 - (1 - pos.y + properties.margin.w);

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

    Widget(const Widget &parent, const widgetProps& props, Shader *shader, Shader *pickShader) : shader(shader), pickShader(pickShader)
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

    uiVertexArr genVertices() const
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

    void draw()
    {
        if (properties.mouse_interactive)
        {
            MousePicking picker;
            picker.Init(SCR_WIDTH, SCR_HEIGHT);
            picker.enable_writing();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //hidden draw call goes here
            pickShader->use();
            int gObjectIndexLoc = glGetUniformLocation(pickShader->ID, "gObjectIndex");
            int gDrawIndexLoc = glGetUniformLocation(pickShader->ID, "gDrawIndex");
            glUniform1i(gObjectIndexLoc, ID);
            glUniform1i(gDrawIndexLoc, 0);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            picker.disable_writing();
        }
        int colourLoc = glGetUniformLocation(shader->ID, "inputColour");
        shader->use();
        glUniform4f(colourLoc, properties.colour.x, properties.colour.y, properties.colour.z, properties.colour.w);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void clean() const
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    float width;
    float height;
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    widgetProps properties;
    Shader* shader;
    Shader* pickShader;
    unsigned int ID;
    unsigned int VAO, VBO, EBO;

};



class Text: public Widget
{
public:
    Text(glm::vec3 position, const widgetProps& props, const std::string& content, glm::vec3 text_colour, TextRenderer* rend, Shader* text_shader, Shader* uiShader, Shader* pick_shader) : Widget(position, props, uiShader, pick_shader)
    {
        text = content;
        colour = text_colour;
        textShader = text_shader;
        renderer = rend;
        pos.x = position.x + props.margin.x;
        pos.y = position.y - props.margin.y;
    }
    Text(const Widget &parent, const widgetProps& props, const std::string& content, glm::vec3 text_colour, TextRenderer* rend, Shader* text_shader, Shader* uiShader, Shader* pick_shader) : Widget(parent, props, uiShader, pick_shader)
    {
        text = content;
        colour = text_colour;
        textShader = text_shader;
        renderer = rend;
        pos.x = parent.pos.x + parent.properties.margin.x + parent.properties.padding.x;
        pos.y = parent.pos.y - parent.properties.margin.y - parent.properties.padding.y;
    }

    void draw()
    {
        glm::vec3 position = unNormalize(pos.x, pos.y, SCR_WIDTH, SCR_HEIGHT);
        renderer->render_text(*textShader, text, position.x, SCR_HEIGHT - position.y, 1.0f, colour);
    }

    void update(std::string content)
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
    TextRenderer* renderer;
    Shader* textShader;
    glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
};

class Image: public Widget
{
public:
    Image(const Widget &parent, const widgetProps& props, Shader* uiShader, Shader* pick_shader) : Widget(parent, props, uiShader, pick_shader)
    {

    }
    Image(glm::vec3 position, const widgetProps& props, Shader* uiShader, Shader* pick_shader) : Widget(position, props, uiShader, pick_shader)
    {

    }
};


class Button: public Widget
{
public:
    Button(const Widget &parent, const widgetProps& props, Shader* uiShader, Shader* pick_shader) : Widget(parent, props, uiShader, pick_shader)
    {

    }

};

class Container: public Widget
{
public:
    Container(glm::vec3 position, const widgetProps& props, Shader* uiShader, Shader* pick_shader) : Widget(position, props, uiShader, pick_shader)
    {

    }
    Container(const Widget &parent, const widgetProps& props, Shader* uiShader, Shader* pick_shader) : Widget(parent, props, uiShader, pick_shader)
    {

    }

};

class Slider: public Widget
{
    Slider(glm::vec3 position, const widgetProps& props, Text* link, Shader* uiShader, Shader* pick_shader) : Widget(position, props, uiShader, pick_shader)
    {
        linkedWidget = link;
    }

    void draw(Shader &uiShader);

private:
    Text* linkedWidget;

};

struct WTNode
{
    Widget* widget;
    std::vector<std::shared_ptr<WTNode>> children;

    WTNode(Widget* widget) : widget(widget) {}

    void Render()
    {
        widget->draw();
        for (std::shared_ptr<WTNode> node : children)
        {
            node->widget->draw();
        }
    }
};


#endif
