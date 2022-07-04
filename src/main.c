#include <stdio.h>
#include <stdlib.h>
#define GLEW_NO_GLU
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <string.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

int fileSize(FILE* file) {
    int size = 0;

    if(file) {
        int prev = ftell(file);
        fseek(file, 0L, SEEK_END);
        size = ftell(file);
        fseek(file, prev, SEEK_SET);
    }

    return size; 
}

void windowFrameBufferCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int createShaderProgram(const char* vs_path, const char* fs_path) {
    FILE* vs_file = fopen(vs_path, "r");
    FILE* fs_file = fopen(fs_path, "r");

    if(vs_file == 0 || fs_file == 0) {
        printf("A shader could not be opened!\n");
        return 0;
    }

    int vs_length = fileSize(vs_file);
    int fs_length = fileSize(fs_file);

    void* vs_buffer = malloc(vs_length * sizeof(char));
    void* fs_buffer = malloc(fs_length * sizeof(char));

    fread(vs_buffer, sizeof(char), vs_length, vs_file);
    fread(fs_buffer, sizeof(char), fs_length, fs_file);

    fclose(vs_file);
    fclose(fs_file);

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const GLchar*[]){(const char *)vs_buffer}, NULL);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const GLchar*[]){(const char *)fs_buffer}, NULL);
    glCompileShader(fragment_shader);

    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glUseProgram(shader_program);


    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    free(vs_buffer);
    free(fs_buffer);

    return shader_program;
}

int main() {
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Rainbow Brick Triangle", 0, 0);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glewInit();

    glfwSetFramebufferSizeCallback(window, windowFrameBufferCallback);

    unsigned int triangle_buffer;
    glGenBuffers(1, &triangle_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 100, 0, GL_DYNAMIC_DRAW);

    unsigned int shader_program = createShaderProgram("resources/shaders/shader.vs", "resources/shaders/shader.fs");

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char *data = stbi_load("resources/wall.jpg", &width, &height, 0, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glfwSwapInterval(1);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        // Render here
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double time = glfwGetTime();

        const float PI = 3.14;
        float x1 = cos(time);
        float y1 = sin(time);
        float x2 = cos(time + (2.0f/3.0f * PI));
        float y2 = sin(time + (2.0f/3.0f * PI));
        float x3 = cos(time + (4.0f/3.0f * PI));
        float y3 = sin(time + (4.0f/3.0f * PI));

        float verts[] = {
            //pos   //colors          //tex cords
            x1, y1, 1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
            x2, y2, 1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, 
            x3, y3, 1.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f,
        };

        glBindBuffer(GL_ARRAY_BUFFER, triangle_buffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glDeleteBuffers(1, &triangle_buffer);
    glDeleteProgram(shader_program);

    glfwTerminate();
}


