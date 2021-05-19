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


static vec3 translation = { 0.0f };
static vec3 scale = { 1.0f };
static vec3 rotation = { 0.0f };
static vec3 last_mouse_pos = { 0.0f };
static int cursor_locked = 1;

static void HandleInput(GLFWwindow* window);
static void HandleCursorPosInput(GLFWwindow* window, double xpos, double ypos);
static void HandleMouseButtonInput(GLFWwindow* window, int button, int action, int mods);

int PiramideApp() {

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

    const int num_vertices = 5;
    const int num_indices = 21;
    
    vec3 base(0.0f);
    vec3 vertexBufferArray[num_vertices] = 
    {
        {0, 1, 0}, {-0.5f, 0, -0.5f}, {-0.5f, 0, 0.5f}, {0.5f, 0, 0.5f}, {0.5f, 0, -0.5f}
    };

    for(auto& vertex : vertexBufferArray)
    {
        std::cout << vertex << std::endl;
    }
    GLuint indexBufferArray[num_indices] = 
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 1,
        1, 2, 3, 
        1, 3, 4
    };
    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * num_vertices, vertexBufferArray, GL_STATIC_DRAW);
    
    GLuint indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * num_indices, indexBufferArray, GL_STATIC_DRAW);

    // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
    GLint loc_position = glGetAttribLocation(shader.GetID(), "position");
    GLint loc_transform = glGetUniformLocation(shader.GetID(), "transformation_matrix");
    GLint loc_color = glGetUniformLocation(shader.GetID(), "color");

    glEnableVertexAttribArray(loc_position);
    glVertexAttribPointer(loc_position, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

    // Hide cursor for rotation
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, HandleCursorPosInput);
    glfwSetMouseButtonCallback(window, HandleMouseButtonInput);
    
    // Exibindo nossa janela
    glfwShowWindow(window);

    while (!glfwWindowShouldClose(window))
    {
        auto start = std::chrono::steady_clock::now();

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        HandleInput(window);

        mat4 model = mat4::Identity();
        model = mat4::translate(translation) * mat4::rotate(rotation) * mat4::scale(scale) * model;

        // Transforma e desenha cilindro
        shader.Bind();
        glUniformMatrix4fv(loc_transform, 1, GL_TRUE, model.m);

        glUniform4f(loc_color, 0, 0, 0, 1);
        glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
        
        glfwSwapBuffers(window);
        
        {
            // Wait for end of frame
            using namespace std::chrono_literals;
            using namespace std::chrono;
            std::this_thread::sleep_for(start - steady_clock::now() + MS_PER_FRAME );
        }
#ifdef DEBUG
        while (int error = glGetError() != GL_NO_ERROR) 
        {
            std::cout << "GlErrorFlag: " << error << std::endl;
        }
#endif // DEBUG
    }

    glfwDestroyWindow(window);
    glfwTerminate();

	return EXIT_SUCCESS;
}

static void HandleInput(GLFWwindow* window) {
    
    vec3 applyTrans;
    applyTrans.x = glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A);
    applyTrans.y = glfwGetKey(window, GLFW_KEY_W) - glfwGetKey(window, GLFW_KEY_S);
    applyTrans.z = glfwGetKey(window, GLFW_KEY_E) - glfwGetKey(window, GLFW_KEY_Q);
    
    vec3 applyScale;
    applyScale.x = glfwGetKey(window, GLFW_KEY_RIGHT) - glfwGetKey(window, GLFW_KEY_LEFT);
    applyScale.y = glfwGetKey(window, GLFW_KEY_UP) - glfwGetKey(window, GLFW_KEY_DOWN);
    applyScale.z = 0.0f;

    translation += applyTrans * TRANSLATION_SPEED;
    scale += applyScale * SCALE_SPEED;
}

static void HandleCursorPosInput(GLFWwindow* window, double xpos, double ypos)
{
    rotation += vec3(ypos - last_mouse_pos.x, xpos - last_mouse_pos.y, 0.0f) * ROTATION_SPEED * cursor_locked;
    
    last_mouse_pos = vec3(ypos, xpos, 0.0f);
}

static void HandleMouseButtonInput(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursor_locked = 1;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            cursor_locked = 0;
        }
    }
}