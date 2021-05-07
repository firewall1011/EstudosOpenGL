/* para windows, instalar bibliotecas compiladas do glfw3 e glew no ambiente mingw */
/* para compilar no windows: gcc main.c -lglfw3dll -lglew32 -lopengl32 */

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>  
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tvector.h"
#include "Shader.h"
#include <iostream>

#define len(x) (sizeof(x) / sizeof(x[0]))
#define PI 3.141592f

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

    TLibrary::Shader shader("coreshader.glsl");
    shader.Bind();
    GLuint program = shader.GetID();

    // Preparando dados para enviar a GPU
    const int num_formas = 9;
    const int resolucao_circulo = 32;

    // Array de deslocamenos entre as formas geometricas
    int deslocamentos[num_formas + 1];
    
    // Array de primitivas a serem desenhadas
    const GLenum primitivas[] = { GL_TRIANGLE_FAN, GL_TRIANGLE_FAN, GL_POINTS, GL_POINTS, GL_TRIANGLE_STRIP, GL_TRIANGLE_STRIP, GL_TRIANGLES, GL_LINE_STRIP, GL_LINE_BIT };
    
    // Array de formas geometricas (array de vertices) a serem desenhadas
    vec2** formas_geometricas = new vec2*[num_formas];

    /* Criar formas geometricas */
    deslocamentos[0] = 0;

    // Olho Piscando
    formas_geometricas[0] = CriarCirculo(resolucao_circulo, .1f, { 0.5f, 0.5f }, PI);
    deslocamentos[1] = resolucao_circulo;

    // Olho Aberto
    formas_geometricas[1] = CriarCirculo(resolucao_circulo, .1f, { -0.5f, 0.5f });
    deslocamentos[2] = deslocamentos[1] + resolucao_circulo;

    // Ponto
    formas_geometricas[2] = new vec2{ 0.5f, 0.5f };
    deslocamentos[3] = deslocamentos[2] + 1;

    // Pontos em Circulo
    formas_geometricas[3] = CriarCirculo(8, 0.05f, { -0.5f, 0.5f });
    deslocamentos[4] = deslocamentos[3] + 8;


    // Sombrancelha Direita
    formas_geometricas[4] = CriarQuadrado(0.05f, { 0.5f, 0.7f });
    deslocamentos[5] = deslocamentos[4] + 4;

    // Sombrancelha Esquerda
    formas_geometricas[5] = CriarQuadrado(0.05f, { -0.5f, 0.7f });
    deslocamentos[6] = deslocamentos[5] + 4;

    // Nariz (Triangulo)
    formas_geometricas[6] = new vec2[]{ {0.0f, -0.05f}, {0.05f, 0.05f}, {-0.05f, 0.05f} };
    deslocamentos[7] = deslocamentos[6] + 3;

    formas_geometricas[7] = CriarCirculo(resolucao_circulo, 0.5f, { 0.0f, 0.0f }, PI);
    deslocamentos[8] = deslocamentos[7] + resolucao_circulo;

    // Barba
    vec2* parte_menor = CriarCirculo(resolucao_circulo, 0.7f, { 0.0f, 0.0f }, PI);
    vec2* parte_maior = CriarCirculo(resolucao_circulo, 0.8f, { 0.0f, 0.0f }, PI);

    // Juntar vertices
    formas_geometricas[8] = new vec2[resolucao_circulo * 2];
    for (int i = 0; i < resolucao_circulo * 2; i += 2) {
        formas_geometricas[8][i] = parte_maior[i / 2];
        formas_geometricas[8][i + 1] = parte_menor[i / 2];
    }
    deslocamentos[9] = deslocamentos[8] + resolucao_circulo * 2;

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // Abaixo, nós enviamos todo o conteúdo da variável vertices.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * deslocamentos[len(deslocamentos) - 1], NULL, GL_DYNAMIC_DRAW);

    for (int i = 0; i < len(primitivas); i++) {
        glBufferSubData(GL_ARRAY_BUFFER, deslocamentos[i] * sizeof(vec2), (deslocamentos[i + 1] - deslocamentos[i]) * sizeof(vec2), (void*)formas_geometricas[i]);
    }

    // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
    GLint loc_position = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

    GLint loc_color = glGetUniformLocation(program, "color");
    std::cout << "Loc color: " << loc_color << std::endl;

    // Exibindo nossa janela
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.4f, 0.0f, 0.6f, 1.0f);
        
        // Desenha primitivas
        for (int i = 0; i < len(primitivas); i++) {
            float t = i / (float)len(primitivas);
            glUniform4f(loc_color, t, t, 1 - t, 1);
            glDrawArrays(primitivas[i], deslocamentos[i], deslocamentos[i + 1] - deslocamentos[i]);
        }

        glfwSwapBuffers(window);
    }

    // Free memory
    for (int i = 0; i < num_formas; i++)
        free(formas_geometricas[i]);

    glfwDestroyWindow(window);

    glfwTerminate();

    exit(EXIT_SUCCESS);
}
