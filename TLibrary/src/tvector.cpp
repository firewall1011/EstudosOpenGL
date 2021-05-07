#include "tvector.h"
#include <cmath>

vec2* CriarCirculo(int num_vertices, float radius, vec2 centro, float angulo_inicial, float angulo_final) {
    vec2* vertices = new vec2[num_vertices];

    float angle = angulo_inicial;

    float x, y;
    for (int i = 0; i < num_vertices; i++)
    {
        x = (cosf(angle) * radius) + centro.x;
        y = (sinf(angle) * radius) + centro.y;
        vertices[i].x = x;
        vertices[i].y = y;
        angle += (angulo_final - angulo_inicial) / (num_vertices - 1);
    }

    return vertices;
}

vec2* CriarQuadrado(float largura, vec2 centro) {
    vec2* vertices = new vec2[4];

    vertices[0] = { centro.x - largura / 2, centro.y - largura / 2 }; // esquerda embaixo
    vertices[1] = { centro.x + largura / 2, centro.y - largura / 2 }; // direita embaixo
    vertices[2] = { centro.x - largura / 2, centro.y + largura / 2 }; // esquerda cima
    vertices[3] = { centro.x + largura / 2, centro.y + largura / 2 }; // direita cima

    return vertices;
}

vec2* CriarRetangulo(float largura, float altura, vec2 centro) {
    vec2* vertices = new vec2[4];

    vertices[0] = { centro.x - largura / 2, centro.y - altura / 2 }; // esquerda embaixo
    vertices[1] = { centro.x + largura / 2, centro.y - altura / 2 }; // direita embaixo
    vertices[2] = { centro.x - largura / 2, centro.y + altura / 2 }; // esquerda cima
    vertices[3] = { centro.x + largura / 2, centro.y + altura / 2 }; // direita cima

    return vertices;
}

vec2 vec2::normalized()
{
    if (magnitude() == 0.0f)
        return { 0.0f, 0.0f };
    return {x / magnitude(), y / magnitude()};
}

float vec2::magnitude()
{
    return sqrtf( x * x + y * y);
}

vec2 vec2::operator*(float mult)
{
    return {x * mult, y * mult};
}

void vec2::operator+=(vec2 other)
{
    x += other.x; 
    y += other.y;
}
