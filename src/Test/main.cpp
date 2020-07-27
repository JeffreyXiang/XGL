#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Math/Transform.h>
#include <Math/View.h>
#include <Math/Projection.h>
#include <Math/Tool.h>
#include <Camera.h>
#include <Texture.h>
#include <Program.h>
#include <Object.h>
#include <stb_image.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

using namespace XGL;

char infoLog[2048];

int screenWidth = 1920;
int screenHeight = 1080;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//------- camera -------
Camera camera;
float cameraSpeed = 2;

//------- cursor -------
float lastX = 400, lastY = 300;
float cursorSensitivity = 0.002;
bool cursorFocus = false;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
    camera.setAspect((float)width / height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (cursorFocus)
    {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
        lastX = xpos;
        lastY = ypos;

        xoffset *= cursorSensitivity;
        yoffset *= cursorSensitivity;

        camera.rotate(xoffset, yoffset);
    }
    else
    {
        cursorFocus = true;
        lastX = xpos;
        lastY = ypos;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.zoom(yoffset * 0.1);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    float cameraStep = cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForwardAl(cameraStep);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveBackwardAl(cameraStep);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveLeftAl(cameraStep);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRightAl(cameraStep);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.moveUpAl(cameraStep);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.moveDownAl(cameraStep);
}

int main()
{
    // ------- Init -------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //------- data --------
    std::vector<Vec3> modelPositions = {
        Vec3( 0.5f,  0.5f,  0.5f),
        Vec3( 0.5f, -0.5f,  0.5f),
        Vec3(-0.5f, -0.5f,  0.5f),
        Vec3(-0.5f,  0.5f,  0.5f),
        Vec3(-0.5f,  0.5f, -0.5f),
        Vec3(-0.5f, -0.5f, -0.5f),
        Vec3( 0.5f, -0.5f, -0.5f),
        Vec3( 0.5f,  0.5f, -0.5f),
        Vec3(-0.5f,  0.5f,  0.5f),
        Vec3(-0.5f, -0.5f,  0.5f),
        Vec3(-0.5f, -0.5f, -0.5f),
        Vec3(-0.5f,  0.5f, -0.5f),
        Vec3( 0.5f,  0.5f, -0.5f),
        Vec3( 0.5f, -0.5f, -0.5f),
        Vec3( 0.5f, -0.5f,  0.5f),
        Vec3( 0.5f,  0.5f,  0.5f),
        Vec3( 0.5f,  0.5f, -0.5f),
        Vec3( 0.5f,  0.5f,  0.5f),
        Vec3(-0.5f,  0.5f,  0.5f),
        Vec3(-0.5f,  0.5f, -0.5f),
        Vec3( 0.5f, -0.5f, -0.5f),
        Vec3( 0.5f, -0.5f,  0.5f),
        Vec3(-0.5f, -0.5f,  0.5f),
        Vec3(-0.5f, -0.5f, -0.5f)
    };

    std::vector<Vec2> modelTexcoords = {
        Vec2(1.0f, 1.0f),
        Vec2(1.0f, 0.0f),
        Vec2(0.0f, 0.0f),
        Vec2(0.0f, 1.0f),
        Vec2(1.0f, 1.0f),
        Vec2(1.0f, 0.0f),
        Vec2(0.0f, 0.0f),
        Vec2(0.0f, 1.0f),
        Vec2(1.0f, 1.0f),
        Vec2(1.0f, 0.0f),
        Vec2(0.0f, 0.0f),
        Vec2(0.0f, 1.0f),
        Vec2(1.0f, 1.0f),
        Vec2(1.0f, 0.0f),
        Vec2(0.0f, 0.0f),
        Vec2(0.0f, 1.0f),
        Vec2(1.0f, 1.0f),
        Vec2(1.0f, 0.0f),
        Vec2(0.0f, 0.0f),
        Vec2(0.0f, 1.0f),
        Vec2(1.0f, 1.0f),
        Vec2(1.0f, 0.0f),
        Vec2(0.0f, 0.0f),
        Vec2(0.0f, 1.0f)
    };

    std::vector<unsigned int> modelIndices = {
        0, 1, 3,
        1, 2, 3,
        4, 5, 7,
        5, 6, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        16, 17, 19,
        17, 18, 19,
        20, 21, 23,
        21, 22, 23,
    };

    Vec3 positions[] = {
        Vec3(0.0f,  0.0f,  0.0f),
        Vec3(2.0f,  5.0f, -15.0f),
        Vec3(-1.5f, -2.2f, -2.5f),
        Vec3(-3.8f, -2.0f, -12.3f),
        Vec3(2.4f, -0.4f, -3.5f),
        Vec3(-1.7f,  3.0f, -7.5f),
        Vec3(1.3f, -2.0f, -2.5f),
        Vec3(1.5f,  2.0f, -2.5f),
        Vec3(1.5f,  0.2f, -1.5f),
        Vec3(-1.3f,  1.0f, -1.5f)
    };

    //setup camera
    camera.setPosition(Vec3(0, 0, 3));
    camera.smoothRotate(0.1);
    camera.smoothZoom(0.1);

    //creat object
    Object object;
    object.setModelPositions(modelPositions);
    object.setModelTexcoords(modelTexcoords);
    object.setModelIndices(modelIndices);

    //load texture
    Texture texture1("../data/container.jpg");
    Texture texture2("../data/awesomeface.png");
    object.addTexture(texture1, "texture0", 0);
    object.addTexture(texture2, "texture1", 1);

    //load glsl programs
    Shader<ShaderType::VERTEX> vertexShader("../src/Test/shaders/shader.vert");
    Shader<ShaderType::FRAGMENT> fragmentShader("../src/Test/shaders/shader.frag");

    //compile glsl program
    vertexShader.compile();
    fragmentShader.compile();

    // creat shader program, bind glsl
    Program program;
    program.setCamera(camera);
    program.attachShader(vertexShader);
    program.attachShader(fragmentShader);
    program.link();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.updateCamera(deltaTime);

        for (int i = 0; i < 10; i++)
        {
            object.setPosition(positions[i]);
            object.setRotation(i, Vec3(1, 0.5, 0.3));
            object.setScaling(1);
            program.draw(object);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
