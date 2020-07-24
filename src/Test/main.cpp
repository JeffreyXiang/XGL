#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Math/Transform.h>
#include <Math/View.h>
#include <Math/Projection.h>
#include <Math/Tool.h>
#include <Camera.h>
#include <stb_image.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

char infoLog[2048];

int screenWidth = 1920;
int screenHeight = 1080;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

//------- camera -------
XGL::Camera camera;
float cameraSpeed = 5;

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

bool compileOutput(unsigned int shader)
{
    int  success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 2048, NULL, infoLog);
        cout << "ERROR: Compilation failed.\n" << infoLog << endl;
    }
    return success;
}

char* openGLSLProgram(const char* filename)
{
    ifstream f;
    stringstream buf;
    f.open(filename);
    if (!f.is_open())
    {
        cout << "ERROR: Cannot open GLSL program.";
        exit(-1);
    }
    buf << f.rdbuf();
    f.close();
    int length = buf.str().size();
    char* res = new char[length];
    strcpy(res, buf.str().c_str());
    return res;
}

bool linkOutput(unsigned int shaderProgram)
{
    int  success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 2048, NULL, infoLog);
        cout << "ERROR: Link failed.\n" << infoLog << endl;
    }
    return success;
}

unsigned int loadTexture(const char* filename, GLenum texID)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(texID); // 在绑定纹理之前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 加载并生成纹理
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 3);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "ERROR: Failed to load texture \"" << filename << "\".\n";
    }
    stbi_image_free(data);
    return texture;
}

int main()
{
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    camera.setPosition(XGL::Vec3(0, 0, 3));
    camera.smoothRotate(0.1);
    camera.smoothZoom(0.1);

    //------- data --------
    float vertices[] = {
        //     ---- 位置 ----      - 纹理坐标 -
             0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
             0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
             0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   0.0f, 1.0f
    };

    unsigned int indices[] = {
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

    XGL::Vec3 positions[] = {
        XGL::Vec3(0.0f,  0.0f,  0.0f),
        XGL::Vec3(2.0f,  5.0f, -15.0f),
        XGL::Vec3(-1.5f, -2.2f, -2.5f),
        XGL::Vec3(-3.8f, -2.0f, -12.3f),
        XGL::Vec3(2.4f, -0.4f, -3.5f),
        XGL::Vec3(-1.7f,  3.0f, -7.5f),
        XGL::Vec3(1.3f, -2.0f, -2.5f),
        XGL::Vec3(1.5f,  2.0f, -2.5f),
        XGL::Vec3(1.5f,  0.2f, -1.5f),
        XGL::Vec3(-1.3f,  1.0f, -1.5f)
    };

    //creat vert buffer
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);

    //load texture
    loadTexture("../data/container.jpg", GL_TEXTURE0);
    loadTexture("../data/awesomeface.png", GL_TEXTURE1);

    //load glsl programs
    char* vertexShaderSource =
        openGLSLProgram("../src/Test/shaders/shader.vert");
    char* fragmentShaderSource =
        openGLSLProgram("../src/Test/shaders/shader.frag");

    //compile glsl program
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    compileOutput(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    compileOutput(fragmentShader);

    // creat shader program, bind glsl
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    linkOutput(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture0"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
    glUseProgram(0);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        camera.update(deltaTime);

        XGL::Mat4& view = camera.viewMat();
        XGL::Mat4& projection = camera.projectionMat();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, view.rawData());
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projection.rawData());

        for (int i = 0; i < 10; i++)
        {
            XGL::Mat4 model = XGL::Mat4::identity();
            XGL::Transform::rotate(model, (float)(i + glfwGetTime()), positions[(i + 5) % 10] + XGL::Vec3(0.1, 0.03, 0.05));
            XGL::Transform::translate(model, positions[i]);
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, model.rawData());
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
        }

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
