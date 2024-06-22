#include <glad.h>
#include <glfw3.h>
#include <utility.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
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
std::vector<float[4]> boundingBoxes;
glm::mat4 projection = glm::perspective(glm::radians(60.0f), 16.0f/9.0f, 0.1f, 100.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = camera.get_view_mat4();


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_orbit_callback(GLFWwindow* window, double xpos, double ypos)
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
};

void hitOrNah(glm::vec3 coords, glm::vec3 ray, int button, int action)
{
    
}

void mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
    {
        glfwSetCursorPosCallback(window, mouse_orbit_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    }
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
    {
        glfwSetCursorPosCallback(window, nullptr);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstMouse = true;

    }
    if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        MousePicking picker;
        glm::vec3 normCoords = picker.normalize(xpos, ypos, SCR_WIDTH, SCR_HEIGHT);
        glm::vec3 ray = picker.getRay(normCoords, model, view, projection, {0, 0, SCR_WIDTH, SCR_HEIGHT});
        hitOrNah(normCoords, ray, button, action);
    }
};

void processInput(GLFWwindow* window, Camera &camera, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
       
    }
    camera.move(window, deltaTime);
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
    glfwSetMouseButtonCallback(window, mouse_click_callback);
    
    return window;
}



int main()
{
    GLFWwindow* window = glfwSetup();
    TextRenderer CMUSerif("./assets/cmunti.ttf", 28);
    AudioSys soundEngine;
    soundEngine.play_single_sound("./assets/alephCAudio.wav");

    Shader uiShader("./shaders/uiVertShader.shad", "./shaders/uiFragShader.shad");
    Shader worldShader("./shaders/worldVertShader.shad", "./shaders/worldFragShader.shad");
    Shader lightShader("./shaders/worldVertShader.shad", "./shaders/lightFragShader.shad");
    Shader textShader("./shaders/textVertShader.shad", "./shaders/textFragShader.shad");
    glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));

    int objectColourLoc, lightColourLoc1, lightColourLoc2;
    objectColourLoc = glGetUniformLocation(worldShader.ID, "objectColour");
    lightColourLoc1 = glGetUniformLocation(worldShader.ID, "lightColour");
    lightColourLoc2 = glGetUniformLocation(lightShader.ID, "lightColour");
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


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
    
    Object3D cube(standard);
    Object3D lightSource(standard);
    
    widgetProps props;
    props.margin = glm::vec4(0.0f, 0.05f, 0.05f, 1.9f);
    props.colour = glm::vec4(0.17f, 0.17f, 0.17f, 1.0f);
    
    glm::vec3 objectColour = glm::vec3(0.5f, 0.4f, 0.5f);
    glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
    
    Widget square(glm::vec3(0.8f, 1.0f, 0.0f), props);
    
    Text FPS(square, props, std::string("FPS: "), {1.0f, 1.0f, 1.0f});
    
    
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
    
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    while(!glfwWindowShouldClose(window))
    {
        view = camera.get_view_mat4();
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        // input checking:
        processInput(window, camera, deltaTime);
        
        // rendering goes here:
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // UI Rendering:
        
        FPS.writeTo(uiShader, CMUSerif, textShader, 1.0f, SCR_WIDTH, SCR_HEIGHT);

        
        // World Rendering:
        lightShader.use();
        glUniform3fv(lightColourLoc2, 1, glm::value_ptr(lightColour));
        
        lightSource.draw(glm::vec3(-1.3f,  1.0f, -1.5f), lightShader, model, view, projection);

        
        worldShader.use();
        glUniform3fv(objectColourLoc, 1, glm::value_ptr(objectColour));
        glUniform3fv(lightColourLoc1, 1, glm::value_ptr(lightColour));

        for(auto cubePosition : cubePositions)
        {
            cube.draw(cubePosition, worldShader, model, view, projection);
        }
        
        
        // frame house-keeping:
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cube.clean();
    
    
    glfwTerminate();
    return 0;
}

