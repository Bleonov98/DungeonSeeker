#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Game.h"

// input
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// mouse
double lastX;

// Program
Game DungeonSeeker(SCR_WIDTH, SCR_HEIGHT);

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "DungeonSeeker", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    gladLoadGL();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    DungeonSeeker.Init();

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.2f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Timing
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // updating
        DungeonSeeker.ProcessInput(deltaTime);
        DungeonSeeker.Update(deltaTime);

        // render
        DungeonSeeker.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (DungeonSeeker.close) glfwSetWindowShouldClose(window, true);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            DungeonSeeker.Keys[key] = true;
        else if (action == GLFW_RELEASE) {
            DungeonSeeker.Keys[key] = false;
            DungeonSeeker.KeysProcessed[key] = false;
        }
    }
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button >= 0 && button < 8)
    {
        if (action == GLFW_PRESS) DungeonSeeker.mouseKeys[button] = true;
        else if (action == GLFW_RELEASE) {
            DungeonSeeker.mouseKeys[button] = false;
            DungeonSeeker.mouseKeysProcessed[button] = false;
        }
    }
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    DungeonSeeker.xMouse = xpos;
    DungeonSeeker.yMouse = ypos;
}