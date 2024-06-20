#include <glad.h>
#include <glfw3.h>
#include <audio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shader.h>
#include <camera.h>
#include <widgets.h>
#include <object.h>
#include <ft2build.h>
#include FT_FREETYPE_H



const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.mouse_callback(window, xoffset, yoffset);
}

void processInput(GLFWwindow* window, Camera &camera, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
       
    }
    camera.move(window, deltaTime);
}

int fontLibSetup(FT_Library &library)
{
    FT_Error error = FT_Init_FreeType(&library);
    if(error)
    {
        std::cout << "An error occured during font initialization\n";
        return -1;
    }
    return 0;
}

GLFWwindow* glfwSetup()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "AlephC", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}




int main()
{
    GLFWwindow* window = glfwSetup();
    // FT_Library fontLib;
    // fontLibSetup(fontLib);
    // FT_Face CMUSerif;
    // FT_New_Face(fontLib, "./assets/cmunci.ttf", 0, &CMUSerif);
    AudioSys soundEngine;
    soundEngine.play_single_sound("./assets/alephCAudio.wav");

    Shader uiShader("./uiVertShader.shad", "./uiFragShader.shad");
    Shader worldShader("./worldVertShader.shad", "./worldFragShader.shad");
    Shader lightShader("./worldVertShader.shad", "./lightFragShader.shad");

    int objectColourLoc, lightColourLoc;
    objectColourLoc = glGetUniformLocation(worldShader.ID, "objectColour");
    lightColourLoc = glGetUniformLocation(worldShader.ID, "lightColour");
    
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
          
    };
    cubeVertexArr standard =
     {{
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
    
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    }};
        
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    Object3D cube(standard);
    Object3D lightSource(standard);
    
    widgetProps props;
    props.margin = glm::vec4(0.0f, 0.05f, 0.05f, 1.9f);
    props.colour = glm::vec4(0.17f, 0.17f, 0.17f, 1.0f);
    
    glm::vec3 objectColour = glm::vec3(0.5f, 0.4f, 0.5f);
    glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
    
    Widget square(glm::vec3(0.8f, 1.0f, 0.0f), props);
    
    while(!glfwWindowShouldClose(window))
    {
        
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        // input checking:
        processInput(window, camera, deltaTime);
        
        // rendering goes here:
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        square.draw(uiShader);
        worldShader.use();
        glUniform3f(objectColourLoc, objectColour.x, objectColour.y, objectColour.z);
        glUniform3f(lightColourLoc, lightColour.x, lightColour.y, lightColour.z);

        for(auto cubePosition : cubePositions)
        {
            cube.draw(cubePosition, worldShader, camera);
        }
        

        lightSource.draw(glm::vec3(-1.3f,  1.0f, -1.5f), lightShader, camera);
        
        
        // frame house-keeping:
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cube.clean();
    square.clean();
    
    glfwTerminate();
    return 0;
}

