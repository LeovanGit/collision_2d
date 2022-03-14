#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../inc/shader.hpp"

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

int main()
{
    glfwInit();
    
    // version 3.3 core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // get main monitor
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();
    // get monitor sizes (videomode)
    const GLFWvidmode * mode = glfwGetVideoMode(monitor);
    // create a fullscreen window
    GLFWwindow * window = glfwCreateWindow(mode->width, mode->height, "Collision demo", monitor, nullptr);
    glfwMakeContextCurrent(window);

    // init glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    float vertices[] =
    {
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,

         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    // Vertex Buffer Objects (stores vertices in GPU memory)
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vertex Arrays Objects (stores VBO attributes condition)
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    
    // set attributes
    glBindVertexArray(VAO);    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); // unbind

    Shader shader("../src/shaders/fragment.frag", "../src/shaders/vertex.vert");

    glClearColor(0.043f, 0.043f, 0.112f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // wiremode
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
