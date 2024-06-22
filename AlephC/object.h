#ifndef OBJECT_H
#define OBJECT_H

#include <glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>
#include <camera.h>

struct cubeVertexArr
{
    float vertices[108];
};

class Object3D
{
public:
    Object3D(const cubeVertexArr& vertices)
    {
        vertexArr = vertices;
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices.vertices), vertices.vertices, GL_STATIC_DRAW);
    
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        
    }

    void draw(glm::vec3 position, Shader &worldShader, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
    {
        worldPos = position;
        modelLoc = glGetUniformLocation(worldShader.ID, "model");
        viewLoc = glGetUniformLocation(worldShader.ID, "view");
        projectionLoc = glGetUniformLocation(worldShader.ID, "projection");

        worldShader.use();
        glBindVertexArray(VAO);

        
        model = glm::translate(model, worldPos);
        
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        glBindVertexArray(0);
    }

    void clean()
    {
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
    }
private:
    glm::vec3 worldPos;
    cubeVertexArr vertexArr;
    int modelLoc, viewLoc, projectionLoc;
    unsigned int VAO, VBO;
};

#endif