#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "physics.hpp"

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

bool pause = false;

// screen resolution compensation
float aspect = 1.0f;

float prev_time = 0;
float delta_time = 0;

void calc_delta_time()
{
    float current_time = glfwGetTime();
    delta_time = current_time - prev_time;
    prev_time = current_time;
}

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

    aspect = static_cast<float>(mode->height) / mode->width;
    
    // hide mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // init glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    float vertices[] =
    {
        // vertices_coords   // texture_coords
        -0.05f, -0.05f, 0.0f, 0.0f, 0.0f,
        -0.05f,  0.05f, 0.0f, 0.0f, 1.0f,
         0.05f,  0.05f, 0.0f, 1.0f, 1.0f,

         0.05f,  0.05f, 0.0f, 1.0f, 1.0f,
         0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f, 0.0f, 0.0f, 0.0f
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
    int t_width, t_height, nr_channels;
    unsigned char * texture_data = stbi_load("../assets/circle.png", 
                                             &t_width, 
                                             &t_height, 
                                             &nr_channels, 
                                             0);

    // bind texture image with texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);

    Shader shader("../src/shaders/fragment.frag", "../src/shaders/vertex.vert");    

    // for transparency (enable alpha channel processing, much affects on performance!)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Ball balls[] =
    {
        // weight  radius  position              speed              acceleration

        // just crazy ball
        Ball(0.5f, 0.0105f, Vec2(-0.95f, 0.5f), Vec2(0.25f, -0.25f), Vec2(0.0f, 0.0f)),

        // ball with gravity
        Ball(1.5f, 0.0315f, Vec2(0.95f, 0.5f), Vec2(-0.2f, -0.2f), Vec2(0.0f, -0.5f)),

        // two big but one without speed
        Ball(2.0f, 0.042f, Vec2(-0.5f, -0.5f), Vec2(0.5f, 0.0f), Vec2(0.0f, 0.0f)),
        Ball(2.0f, 0.042f, Vec2( 0.5f, -0.5f), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)),

        // two big with equal speeds
        Ball(2.0f, 0.042f, Vec2(-0.5f, -0.4f), Vec2( 0.1f, 0.0f), Vec2(0.0f, 0.0f)),
        Ball(2.0f, 0.042f, Vec2( 0.5f, -0.4f), Vec2(-0.1f, 0.0f), Vec2(0.0f, 0.0f)),

        // small and big
        Ball(1.0f, 0.021f, Vec2(-0.5f, -0.3f), Vec2( 0.3f, 0.0f), Vec2(0.0f, 0.0f)),
        Ball(2.0f, 0.042f, Vec2( 0.5f, -0.3f), Vec2(-0.1f, 0.0f), Vec2(0.0f, 0.0f)),

        // three in a row
        Ball(1.0f, 0.021f, Vec2( 0.0f, -0.2f), Vec2( 0.0f, 0.0f), Vec2(0.0f, 0.0f)),
        Ball(1.0f, 0.021f, Vec2(-0.5f, -0.2f), Vec2( 0.3f, 0.0f), Vec2(0.0f, 0.0f)),
        Ball(2.0f, 0.042f, Vec2( 0.5f, -0.2f), Vec2(-0.3f, 0.0f), Vec2(0.0f, 0.0f)),

        // vertical
        Ball(1.0f, 0.021f, Vec2(-0.9f, 0.0f), Vec2(0.0f, -0.2f), Vec2(0.0f, 0.0f)),
        Ball(1.0f, 0.021f, Vec2(-0.9f, -0.3f), Vec2(0.0f, 0.1f), Vec2(0.0f, 0.0f)),
        Ball(3.0f, 0.063f, Vec2(-0.9f, 0.9f), Vec2(0.0f, -0.05f), Vec2(0.0f, 0.0f)),

        // non-central with equal speeds
        Ball(2.0f, 0.042f, Vec2(-0.3f, 0.9f), Vec2(0.3f, 0.0f), Vec2(0.0f, 0.0f)),
        Ball(2.0f, 0.042f, Vec2(0.3f, 0.9f - 0.042f), Vec2(-0.3f, 0.0f), Vec2(0.0f, 0.0f)),

        // non-central, but one without speed
        Ball(2.0f, 0.042f, Vec2(-0.3f, 0.7f), Vec2(0.45f, 0.0f), Vec2(0.0f, 0.0f)),
        Ball(2.0f, 0.042f, Vec2(0.3f, 0.7f - 0.042f), Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)),

    };

    glClearColor(0.043f, 0.043f, 0.112f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // draw
        for (size_t i = 0, size = sizeof(balls) / sizeof(Ball); i != size; i++)
        {
            shader.use();
            // transformations (scale -> rotate -> translate, read from down to up)
            glm::mat4 model_matrix = glm::mat4(1.0f);
            model_matrix = glm::translate(model_matrix, glm::vec3(balls[i].getPosition().getX(),
                                                                  balls[i].getPosition().getY() / aspect,
                                                                  0.0f));
            // size depends on weight
            model_matrix = glm::scale(model_matrix, glm::vec3(balls[i].getWeight()));
            // compensation from screen resolution
            model_matrix = glm::scale(model_matrix, 
                                      glm::vec3(aspect, 1.0f, 1.0f));

            glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model_matrix"),
                               1,
                               GL_FALSE,
                               glm::value_ptr(model_matrix));

            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            for (size_t j = 0; j != size; ++j)
            {
                // self collision
                if (i == j) continue;

                if (balls[i].checkCollision(balls[j]))
                {
                    // std::cout << "collision detected\n";
                    calculateSpeedAfterCollision(balls[i], balls[j]);
                }
            }

            if (!pause) balls[i].move();
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
        glfwSetKeyCallback(window, key_callback);
        calc_delta_time();
        // std::cout << "FPS: " << int(1 / delta_time) << "\n";
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

    // pause
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        pause = true;
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        pause = false;

    // wiremode
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
