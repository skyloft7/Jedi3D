// Jedi3D.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Window.h"
#include <memory>
#include <string>
#include "Files.h"


static int CreateShader(unsigned int type, const std::string& source) {


    unsigned int id = glCreateShader(type);

    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    return id;
}

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}


int main()
{


    if (!glfwInit()) {
        std::cout << "Failed to init GLFW" << std::endl;
    }
    std::unique_ptr<Window> window(new Window(800, 800, "Jedi3D"));
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to init GLEW" << std::endl;
    }



    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    int computeShaderProgram = 0;
    {

        unsigned int textureID;

        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, window->m_width, window->m_height, 0, GL_RGBA,
            GL_FLOAT, NULL);

        glBindImageTexture(0, textureID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

        

        computeShaderProgram = glCreateProgram();
        unsigned int cs = CreateShader(GL_COMPUTE_SHADER, Files::ReadFile("ComputeShader.glsl"));
        glAttachShader(computeShaderProgram, cs);
        glLinkProgram(computeShaderProgram);





    }
    int renderProgram = 0;
    {
        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        float vertices[32] = {
            -1, 1, 1, 0,
            -1, -1, 1, 1,
            1, -1, 0, 1,

            1, -1, 0, 1,
            1, 1, 0, 0,
            -1, 1, 1, 0,
        };



        glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_STATIC_DRAW);

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glVertexAttribPointer(0, 2, GL_FLOAT, false, 4 * sizeof(float), 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, false, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);


        renderProgram = glCreateProgram();
        unsigned int vs = CreateShader(GL_VERTEX_SHADER, Files::ReadFile("VertexShader.glsl"));
        unsigned int fs = CreateShader(GL_FRAGMENT_SHADER, Files::ReadFile("FragmentShader.glsl"));

        glAttachShader(renderProgram, vs);
        glAttachShader(renderProgram, fs);

        glLinkProgram(renderProgram);
        glUseProgram(renderProgram);

        int sampler2DLocation = glGetUniformLocation(renderProgram, "texture_sampler");
        glUniform1i(sampler2DLocation, 0);
    }

    while (!window->shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0, 1.0, 1.0, 0);

        glUseProgram(computeShaderProgram);
        glUniform1f(glGetUniformLocation(computeShaderProgram, "width"), window->m_width);
        glUniform1f(glGetUniformLocation(computeShaderProgram, "height"), window->m_height);


        glDispatchCompute(window->m_width, window->m_height, 1);

        glUseProgram(renderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        window->update();
    }

 
    window->close();

    return 0;
}
