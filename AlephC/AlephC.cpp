#include <glad.h>
#include <glfw3.h>
#include <utility.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shader.h>
#include <camera.h>
#include <widgets.h>
#include <object.h>


Camera camera;
MousePicking picker;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;



glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
glm::mat4 projection = glm::perspective(glm::radians(60.0f), 16.0f/9.0f, 0.1f, 100.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = camera.get_view_mat4();

void initShaders()
{
    
}

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
    Shader uiPickShader("./shaders/uiVertShader.shad", "./shaders/pickingFrag.shad");
    Shader worldShader("./shaders/worldVertShader.shad", "./shaders/worldFragShader.shad");
    Shader lightShader("./shaders/worldVertShader.shad", "./shaders/lightFragShader.shad");
    Shader textShader("./shaders/textVertShader.shad", "./shaders/textFragShader.shad");
    
    int objectColourLoc, lightColourLoc1, lightColourLoc2;
    objectColourLoc = glGetUniformLocation(worldShader.ID, "objectColour");
    lightColourLoc1 = glGetUniformLocation(worldShader.ID, "lightColour");
    lightColourLoc2 = glGetUniformLocation(lightShader.ID, "lightColour");

    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    picker.Init(SCR_WIDTH, SCR_HEIGHT);

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

    //UI Building:
    unsigned int elements = 0;
    widgetProps main_box_props{++elements, false, glm::vec4(0.01f), glm::vec4(0.0f)};
    Container main_box(glm::vec3(-1.0f, 1.0f, 0.0f), main_box_props, &uiShader, &uiPickShader);
    
    widgetProps drawer_box_props{++elements, false, glm::vec4(0.01f), glm::vec4(0.0f), glm::vec4(0.0f)};
    Container drawer(main_box, drawer_box_props, &uiShader, &uiPickShader);

    widgetProps test_props{++elements, true, glm::vec4(0.0f), glm::vec4(0.0f, 0.05f, 0.05f, 1.9f), glm::vec4(0.17f, 0.17f, 0.17f, 1.0f)};
    Widget square(glm::vec3(0.8f, 1.0f, 0.0f), test_props, &uiShader, &uiPickShader);

    widgetProps text_props{++elements, false, glm::vec4(0.0f), glm::vec4(0.0f, 0.05f, 0.05f, 1.9f)};
    Text FPS(square, text_props, std::string("FPS: "), {1.0f, 1.0f, 1.0f}, &CMUSerif, &textShader, &uiShader, &uiPickShader);
    Text GreekLetterTest({-0.8f, 1.0f, 0.0f}, text_props, std::string("Ω"), {1.0f, 1.0f, 1.0f}, &CMUSerif, &textShader, &uiShader, &uiPickShader);

    
    //Scene Building:
    Cube cube(standard);
    Cube lightSource(standard);

    Line zToz({0.0f, 0.0f, -50.0f}, {0.0f, 0.0f, 50.0f}, worldShader);
    Line xTox({-50.0f, 0.0f, 0.0f}, {50.0f, 0.0f, 0.0f}, worldShader);

    glm::vec3 tertiaryColour = glm::vec3(0.51f, 0.54f, 0.55f);
    glm::vec3 objectColour = glm::vec3(0.5f, 0.4f, 0.5f);
    glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 1.0f);
    
    
   

    
    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;
    float interval = 0.0f;
    int framesPerSecond = 0;

    while(!glfwWindowShouldClose(window))
    {
        view = camera.get_view_mat4();
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        if(currentTime - interval > 1)
        {
            interval = currentTime;
            FPS.update(std::string("FPS: " + std::to_string(framesPerSecond)));
            framesPerSecond = 0;
        }
        
        // input checking:
        processInput(window, camera, deltaTime);
        
        // RENDERING:
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // UI Rendering:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        FPS.draw();
        GreekLetterTest.draw();
        
        // World Rendering:
        lightShader.use();
        glUniform3fv(lightColourLoc2, 1, glm::value_ptr(lightColour));
        lightSource.draw(glm::vec3(-1.3f,  1.0f, -1.5f), lightShader, model, view, projection);

        
        worldShader.use();
        glUniform3fv(lightColourLoc1, 1, glm::value_ptr(lightColour));

        glUniform3fv(objectColourLoc, 1, glm::value_ptr(tertiaryColour));
        for (int i = -50; i < 51; i++)
        {
            zToz.draw({i, 0.0f, 0.0f},model, view, projection);
            xTox.draw({0.0f, 0.0f, i}, model, view, projection);
        }
        
        // frame house-keeping:
        framesPerSecond++;
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cube.clean();

    
    glfwTerminate();
    return 0;
}

