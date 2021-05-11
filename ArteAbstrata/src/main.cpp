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

unsigned int matrix_multiplication(float* m1, float* m2, vec2 dim1, vec2 dim2, float* result_m);

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
    TLibrary::Shader blue_shader("res/shaders/color.shader");
    TLibrary::Shader red_shader("res/shaders/red.shader");
    
    program = blue_shader.GetID();

    // Cria Triangulo
    vec2 triangulo[3] = { {0.0f, 0.0f}, {0.1f, 0.0f}, {0.1f, 0.1f} };


    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Abaixo, nós enviamos todo o conteúdo da variável vertices.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 3, triangulo, GL_STATIC_DRAW);


    // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
    GLuint loc_position = glGetAttribLocation(program, "position");
    GLuint loc_transform = glGetUniformLocation(program, "transformation_matrix");
    GLuint loc_color = glGetUniformLocation(program, "color");

    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

    // Associando nossa janela com eventos de mouse
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Exibindo nossa janela
    glfwShowWindow(window);
    int i = 0;
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        s_x += delta_scale;
        if (s_x <= 0.0f)
            s_x = 0.0f;
        float translation[16] = {
                1.0f, 0.0f, 0.0f, 0.25f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        float cosx = cosf(3.1415f / 2);
        float senx = sinf(3.1415f / 2);

        float rotation[16] = {
                cosx, -senx, 0.0f, 0.0,
                senx, cosx, 0.0f, 0.0,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };

        float transform[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        float result[16] = { 0 };
        float result_2[16] = { 0 };
        float result_3[16] = { 0 };
        float result_4[16] = { 0 };

        // Desenha primitivas
        
        blue_shader.Bind();
        
        
        if (i > 7500) {
            matrix_multiplication(rotation, translation, { 4, 4 }, { 4, 4 }, result_3);
            glUniformMatrix4fv(loc_transform, 1, GL_TRUE, result_3);
        }
        else if (i > 5000) {
            matrix_multiplication(rotation, transform, { 4, 4 }, { 4, 4 }, result_3);
            glUniformMatrix4fv(loc_transform, 1, GL_TRUE, result_3);
        }
        else if (i > 2500) {
            matrix_multiplication(translation, rotation, { 4, 4 }, { 4, 4 }, result_2);
            glUniformMatrix4fv(loc_transform, 1, GL_TRUE, result_2);
        }
        else 
        {
            matrix_multiplication(translation, transform, { 4, 4 }, { 4, 4 }, result);
            glUniformMatrix4fv(loc_transform, 1, GL_TRUE, result);
        }

        glUniform4f(loc_color, i / 10000.0f, 0.0, 0.5f, 1.0); //RED
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);

        i++;
        if (i >= 10000)
            i = 0;
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return(EXIT_SUCCESS);
}

unsigned int matrix_multiplication(float* m1, float* m2, vec2 dim1, vec2 dim2, float* result_m) {
    if (dim1.y != dim2.x || result_m == NULL)
        return 1;

    for (int i = 0; i < dim1.x; i++)
    {
        for (int j = 0; j < dim2.y; j++)
        {
            for (int k = 0; k < dim1.y; k++)
            {
                result_m[(int)(i * dim1.x + j)] += (m1[(int)(i * dim1.x + k)] * m2[(int)(k * dim2.x + j)]);
            }
        }
    }

    return 0;
}