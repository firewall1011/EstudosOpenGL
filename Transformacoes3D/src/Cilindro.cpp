#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>  
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "Shader.h"
#include "tvector.h"
#include "mat4.h"
#include "Exercicios.h"

static vec3 rotation = { 0.0f };

static void HandleInput(GLFWwindow* window);

int CilindroApp() {

    // inicicializando o sistema de\ janelas
    glfwInit();

    // deixando a janela invisivel, por enquanto
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // criando uma janela
    GLFWwindow* window = glfwCreateWindow(512, 512, "Minha Janela", NULL, NULL);

    // tornando a janela como principal 
    glfwMakeContextCurrent(window);

    // inicializando Glew (para lidar com funcoes OpenGL)
    GLint GlewInitResult = glewInit();
    printf("GlewStatus: %s\n", glewGetErrorString(GlewInitResult));

    glEnable(GL_DEPTH_TEST);

    TLibrary::Shader shader("res/common.shader");

    const int num_vertices = 32;
    const int num_indices = num_vertices * 2;
    
    vec3 base = { 0.0f, 0.25f, 0.0f };
    vec3* vertexBufferArray = CriarCilindro(num_vertices, 0.25f, 0.5f, base);

    GLuint* indexBufferArray = new GLuint[num_indices];

    for (unsigned int i = 0; i < num_vertices; i++)
        indexBufferArray[i] = i;
    for (unsigned int i = 0, j = 0; i < num_vertices; i+=2, j++)
    {
        int lower_circle_index = j + num_vertices;
        int upper_circle_index = j + (num_vertices * 1.5f);
        
        indexBufferArray[lower_circle_index] = i;
        indexBufferArray[upper_circle_index] = i+1;
    }

    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * num_vertices, vertexBufferArray, GL_STATIC_DRAW);
    
    
    GLuint indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_indices, indexBufferArray, GL_STATIC_DRAW);

    // Associando vari?veis do programa GLSL (Vertex Shaders) com nossos dados
    GLint loc_position = glGetAttribLocation(shader.GetID(), "position");
    GLint loc_transform = glGetUniformLocation(shader.GetID(), "transformation_matrix");
    GLint loc_color = glGetUniformLocation(shader.GetID(), "color");

    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

    // Exibindo nossa janela
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window))
    {
        auto start = std::chrono::steady_clock::now();

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        HandleInput(window);

        mat4 model = mat4::Identity();
        model = mat4::rotateX(rotation.x) * mat4::rotateY(rotation.y) * mat4::rotateZ(rotation.z) * model;

        // Transforma e desenha cilindro
        shader.Bind();
        glUniformMatrix4fv(loc_transform, 1, GL_TRUE, model.m);

        glUniform4f(loc_color, 1, 0, 0, 1);
        glDrawElements(GL_TRIANGLE_STRIP, num_vertices, GL_UNSIGNED_INT, nullptr);
        
        glUniform4f(loc_color, 0, 1, 0, 1);
        glDrawElements(GL_TRIANGLE_FAN, num_vertices/2, GL_UNSIGNED_INT, (const void*) (num_vertices * sizeof(GLuint)));
        
        glUniform4f(loc_color, 0, 0, 1, 1);
        glDrawElements(GL_TRIANGLE_FAN, num_vertices/2, GL_UNSIGNED_INT, (const void*) ((num_vertices + num_vertices / 2)*sizeof(GLuint)));

        glfwSwapBuffers(window);
        
        {
            // Wait for end of frame
            using namespace std::chrono_literals;
            using namespace std::chrono;
            std::this_thread::sleep_for(start + MS_PER_FRAME - steady_clock::now());
        }
#ifdef DEBUG
        while (int error = glGetError() != GL_NO_ERROR) 
        {
            std::cout << "GlErrorFlag: " << error << std::endl;
        }
#endif // DEBUG

    }

    delete[] vertexBufferArray;
    delete[] indexBufferArray;

    glfwDestroyWindow(window);
    glfwTerminate();

	return EXIT_SUCCESS;
}

static void HandleInput(GLFWwindow* window) {
    vec3 applyRot;
    
    applyRot.y = glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A);
    applyRot.x = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
    applyRot.z = glfwGetKey(window, GLFW_KEY_E) - glfwGetKey(window, GLFW_KEY_Q);
    
    rotation += applyRot * ROTATION_SPEED;
}