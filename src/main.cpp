#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
        
        glfwSwapBuffers(window);

        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
    }

    // glfwDestroyWindow(window); // included in glfwTerminate()
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
