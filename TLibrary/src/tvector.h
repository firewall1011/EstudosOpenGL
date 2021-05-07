#pragma once
struct vec2
{
    float x, y;

    vec2 normalized();
    float magnitude();
    vec2 operator*(float);
    void operator+=(vec2);
};

#define T_PI 3.14159f

vec2* CriarCirculo(int num_vertices = 8, float raio = 1.0, vec2 centro = { 0,0 }, float angulo_inicial = 0.0, float angulo_final = 2 * T_PI);
vec2* CriarQuadrado(float largura, vec2 centro);
vec2* CriarRetangulo(float largura, float altura, vec2 centro);