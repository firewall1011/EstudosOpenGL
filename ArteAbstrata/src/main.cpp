#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>  
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include "tvector.h"
#include "Shader.h"


#define SCALE_FACTOR 0.0001f

enum class color {
    blue, red
};

color _color = color::blue;
float s_x = 1.0f;
float delta_scale = 0.0f;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            delta_scale = -SCALE_FACTOR;
            _color = color::blue;
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT) {
            delta_scale = SCALE_FACTOR;
            _color = color::red;
        }
    }
    else if (action == GLFW_RELEASE)
        delta_scale = 0.0f;
}

int main(void) {

    // inicicializando o sistema de\ janelas
    glfwInit();

    // deixando a janela invisivel, por enquanto
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);


    // criando uma janela
    GLFWwindow* window = glfwCreateWindow(600, 600, "Minha Janela", NULL, NULL);


    // tornando a janela como principal 
    glfwMakeContextCurrent(window);

    // inicializando Glew (para lidar com funcoes OpenGL)
    GLint GlewInitResult = glewInit();
    printf("GlewStatus: %s", glewGetErrorString(GlewInitResult));
        
    GLuint program;
    TLibrary::Shader blue_shader("res/shaders/blue.shader");
    TLibrary::Shader red_shader("res/shaders/red.shader");
    
    program = blue_shader.GetID();

    // Cria Triangulo
    vec2 triangulo[3] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f} };


    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Abaixo, nós enviamos todo o conteúdo da variável vertices.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 3, triangulo, GL_STATIC_DRAW);


    // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
    GLuint loc_position = glGetAttribLocation(program, "position");
    GLuint loc_transform = glGetUniformLocation(program, "transformation_matrix");

    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

    // Associando nossa janela com eventos de mouse
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Exibindo nossa janela
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        s_x += delta_scale;
        if (s_x <= 0.0f)
            s_x = 0.0f;
        float tranform[16] = {
                s_x, 0.0f, 0.0f, 0.0f,
                0.0f, s_x, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        glUniformMatrix4fv(loc_transform, 1, GL_TRUE, tranform);
        // Desenha primitivas
        if (_color == color::red)
            red_shader.Bind();
        else if (_color == color::blue)
            blue_shader.Bind();
        
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return(EXIT_SUCCESS);
}