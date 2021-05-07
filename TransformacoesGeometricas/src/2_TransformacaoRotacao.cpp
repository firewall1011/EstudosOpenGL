#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>  
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include "tvector.h"
#include "Exercises.h"


namespace Ex2 {

#define ROTATION_FACTOR 0.0001f

    float theta = 1.0f;
    float delta_rotation = 0.0f;

    void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            if (key == GLFW_KEY_RIGHT)
                delta_rotation = -ROTATION_FACTOR;
            else if (key == GLFW_KEY_LEFT)
                delta_rotation = ROTATION_FACTOR;
        }
        else if (action == GLFW_RELEASE)
            delta_rotation = 0.0f;
    }

    int Exercise_2() {

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


        // GLSL para Vertex Shader
        const char* vertex_code = new char[]
        {
            "attribute vec2 position;\n"
                "uniform mat4 transformation_matrix;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = transformation_matrix * vec4(position, 0.0, 1.0);\n"
                "}\n"
        };

        // GLSL para Fragment Shader
        const char* fragment_code = new char[]
        {
            "uniform vec4 color;\n"
                "void main()\n"
                "{\n"
                "    gl_FragColor = vec4(1.0, 0.2, 0.1, 1.0);\n"
                "}\n"
        };

        // Requisitando slot para a GPU para nossos programas Vertex e Fragment Shaders
        GLuint program = glCreateProgram();
        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

        // Associando nosso código-fonte GLSL aos slots solicitados
        glShaderSource(vertex, 1, &vertex_code, NULL);
        glShaderSource(fragment, 1, &fragment_code, NULL);

        // Compilando o Vertex Shader e verificando erros
        glCompileShader(vertex);

        GLint isCompiled = 0;
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {

            //descobrindo o tamanho do log de erro
            int infoLength = 512;
            glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &infoLength);

            //recuperando o log de erro e imprimindo na tela
            char* info = (char*)malloc(sizeof(char) * infoLength);
            glGetShaderInfoLog(vertex, infoLength, NULL, info);

            printf("Erro de compilacao no Vertex Shader.\n");
            printf("--> %s\n", info);
            free(info);
        }

        // Compilando o Fragment Shader e verificando erros
        glCompileShader(fragment);

        isCompiled = 0;
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {

            //descobrindo o tamanho do log de erro
            int infoLength = 512;
            glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLength);

            //recuperando o log de erro e imprimindo na tela
            char* info = (char*)malloc(sizeof(char) * infoLength);
            glGetShaderInfoLog(fragment, infoLength, NULL, info);

            printf("Erro de compilacao no Fragment Shader.\n");
            printf("--> %s\n", info);
            free(info);
        }

        // Associando os programas compilado ao programa principal
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        // Linkagem do programa e definindo como default
        glLinkProgram(program);
        glUseProgram(program);

        // Cria Triangulo
        vec2* quadrado = CriarQuadrado(0.8f, { 0.0f, 0.0f });


        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, quadrado, GL_STATIC_DRAW);


        // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
        GLint loc_position = glGetAttribLocation(program, "position");
        GLint loc_transform = glGetUniformLocation(program, "transformation_matrix");

        glEnableVertexAttribArray(loc_position);
        glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

        // Associando nossa janela com eventos de teclado
        glfwSetKeyCallback(window, keyboard_callback);


        // Exibindo nossa janela
        glfwShowWindow(window);

        while (!glfwWindowShouldClose(window))
        {

            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            theta += delta_rotation;
            float cost = cosf(theta);
            float sint = sinf(theta);

            float tranform[16] = {
                    cost, -sint, 0.0f, 0.0f,
                    sint, cost,  0.0f, 0.0f,
                    0.0f, 0.0f,  1.0f, 0.0f,
                    0.0f, 0.0f,  0.0f, 1.0f
            };

            glUniformMatrix4fv(loc_transform, 1, GL_TRUE, tranform);
            // Desenha primitivas

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


            glfwSwapBuffers(window);
        }

        glfwDestroyWindow(window);

        glfwTerminate();

        return(EXIT_SUCCESS);
    }
}