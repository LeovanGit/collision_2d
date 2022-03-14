#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

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
        // vertices_coords   // texture_coords
        -0.1f, -0.1f, 0.0f, 0.0f, 0.0f,
        -0.1f,  0.1f, 0.0f, 0.0f, 1.0f,
         0.1f,  0.1f, 0.0f, 1.0f, 1.0f,

         0.1f,  0.1f, 0.0f, 1.0f, 1.0f,
         0.1f, -0.1f, 0.0f, 1.0f, 0.0f,
        -0.1f, -0.1f, 0.0f, 0.0f, 0.0f
    };
    // texture_coords starts from the left down corner!

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0); // unbind

    // TEXTURE
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // texture wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load texture image
    int t_width, t_height, nrChannels;
    unsigned char * texture_data = stbi_load("../assets/circle.png", 
                                             &t_width, 
                                             &t_height, 
                                             &nrChannels, 
                                             0);

    // bind texture image with texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);

    Shader shader("../src/shaders/fragment.frag", "../src/shaders/vertex.vert");    

    // for transparency (enable alpha channel processing, much affects on performance!)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.043f, 0.043f, 0.112f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        glBindTexture(GL_TEXTURE_2D, texture);
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
