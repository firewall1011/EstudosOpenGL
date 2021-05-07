#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>  
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <iostream>
#include "tvector.h"
#include "Exercises.h"

namespace Ex5 {

#define MOVEMENT_SPEED 0.0005f
#define ROTATION_SPEED 0.0015f
#define SCALE_SPEED 0.1f

    vec2 translation = { 0, 0 };
    vec2 scale = { 1.0f, 1.0f };
    float rotation = 0.0f;

    vec2 t_delta = { 0, 0 };
    float s_delta = 0.0f;
    float r_delta = 0.0f;

    void Handle_Movement(GLFWwindow* window);
    void Handle_Rotation(GLFWwindow* window);
    void Handle_Scale(float offset);
    void Handle_Inputs(GLFWwindow* window) {
        Handle_Movement(window);
        Handle_Rotation(window);
    }

    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Handle_Scale(yoffset);
    }

    unsigned int matrix_multiplication(float* m1, float* m2, vec2 dim1, vec2 dim2, float* result_m);

    int Exercise_5() {

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
                "    gl_FragColor = color;\n"
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

        // Cria Circulo
        vec2* circulo = CriarCirculo(64, 0.1f, { 0.0f, 0.0f });
        vec2* quadrado = CriarRetangulo(0.05f, 0.2f, { 0.1f, 0.1f });
        vec2 object_center = { 0, 0 };

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);

        // Abaixo, nós enviamos todo o conteúdo da variável vertices.
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 68, circulo, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec2) * 64, sizeof(vec2) * 4, quadrado);


        // Associando variáveis do programa GLSL (Vertex Shaders) com nossos dados
        GLint loc_position = glGetAttribLocation(program, "position");
        GLint loc_transform = glGetUniformLocation(program, "transformation_matrix");
        GLint loc_color = glGetUniformLocation(program, "color");

        glEnableVertexAttribArray(loc_position);
        glVertexAttribPointer(loc_position, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

        // Exibindo nossa janela
        glfwShowWindow(window);
        glfwSetScrollCallback(window, scroll_callback);

        while (!glfwWindowShouldClose(window))
        {

            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            Handle_Inputs(window);

            translation += t_delta;
            rotation += r_delta;

            float transformation_matrix[16] = { 0 };

            float tranform_translation[16] = {
                    1.0f, 0.0f, 0.0f, translation.x,
                    0.0f, 1.0f, 0.0f, translation.y,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
            };

            float transform_scale[16] = {
                    scale.x, 0.0f,    0.0f, object_center.x * (1 - scale.x),
                    0.0f,    scale.y, 0.0f, object_center.y * (1 - scale.y),
                    0.0f,    0.0f,    0.0f, 0.0f,
                    0.0f,    0.0f,    0.0f, 1.0f
            };

            float cosx = cosf(rotation);
            float sinx = sinf(rotation);

            float transform_rotation[16] = {
                    cosx, -sinx, 0.0f, object_center.x - object_center.x * cosx + object_center.y * sinx,
                    sinx, cosx,  0.0f, object_center.y - object_center.y * cosx - object_center.x * sinx,
                    0.0f, 0.0f,  0.0f, 0.0f,
                    0.0f, 0.0f,  0.0f, 1.0f
            };

            unsigned int error = 0;
            error += matrix_multiplication(tranform_translation, transform_rotation, { 4, 4 }, { 4, 4 }, transformation_matrix);
            error += matrix_multiplication(transformation_matrix, transform_scale, { 4, 4 }, { 4, 4 }, transformation_matrix);
            if (error) {
                std::cout << "Falha na multiplicacao\n";
                break;
            }

            // Transforma e desenha circulo
            glUniform4f(loc_color, 1, 0, 0, 1);
            glUniformMatrix4fv(loc_transform, 1, GL_TRUE, transformation_matrix);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 64);


            // Transforma e desenha quadrado
            glUniform4f(loc_color, 0, 0, 1, 1);
            glDrawArrays(GL_TRIANGLE_STRIP, 64, 4);

            glfwSwapBuffers(window);
        }

        // Limpar memoria
        free(circulo);
        free(quadrado);

        glfwDestroyWindow(window);

        glfwTerminate();

        return EXIT_SUCCESS;
    }

    void Handle_Movement(GLFWwindow* window) {
        vec2 direction = { 0, 0 };

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            direction.x -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            direction.x += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            direction.y += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            direction.y -= 1.0f;

        t_delta = direction.normalized() * MOVEMENT_SPEED;
    }

    void Handle_Rotation(GLFWwindow* window) {
        float direction = 0.0f;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            direction += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            direction -= 1.0f;

        r_delta = direction * ROTATION_SPEED;
    }

    void Handle_Scale(float offset) {
        s_delta = offset * SCALE_SPEED;
        scale += { s_delta, s_delta};
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
}