#ifndef WIDGET_H
#define WIDGET_H

#include <glad.h>
#include <glm/glm.hpp>
#include <array>
#include <shader.h>


struct widgetProps {
    glm::vec4 padding = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 margin = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    int index = 0;
};

struct vertexArr
{
    float vertices[12];
};

class Widget
{
public:
    Widget(glm::vec3 position, widgetProps props)
    {
        pos = position;
        pos.x += props.margin.x;
        pos.y -= props.margin.y;
        width = 1 - (pos.x + props.margin.z);
        height = 2 - (1 - pos.y + props.margin.w);
        
        vertexArr vert;
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
        pos.x = parent.pos.x + parent.padding.x + props.margin.x;
        pos.y = parent.pos.y - parent.padding.y - props.margin.y;
        width = parent.width - (parent.padding.x + parent.padding.z + props.margin.x + props.margin.z);
        height = parent.height - (parent.padding.y + parent.padding.w + props.margin.y + props.margin.w);

        vertexArr vert;
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

    vertexArr genVertices()
    {
        vertexArr vert;
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

    void draw(Shader shader)
    {
        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void clean()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
private:
    float width;
    float height;
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 margin;
    glm::vec4 padding;
    unsigned int VAO, VBO, EBO;
};

#endif