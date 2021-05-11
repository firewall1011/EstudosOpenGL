#include "tvector.h"
#include <cmath>

vec2* CriarCirculo(int num_vertices, float radius, const vec2& centro, float angulo_inicial, float angulo_final) {
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

vec2* CriarQuadrado(float largura, const vec2& centro) {
    vec2* vertices = new vec2[4];

    vertices[0] = { centro.x - largura / 2, centro.y - largura / 2 }; // esquerda embaixo
    vertices[1] = { centro.x + largura / 2, centro.y - largura / 2 }; // direita embaixo
    vertices[2] = { centro.x - largura / 2, centro.y + largura / 2 }; // esquerda cima
    vertices[3] = { centro.x + largura / 2, centro.y + largura / 2 }; // direita cima

    return vertices;
}

vec2* CriarRetangulo(float largura, float altura, const vec2& centro) {
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

void vec2::operator+=(const vec2& other)
{
    x += other.x; 
    y += other.y;
}


vec3* CriarCilindro(const int num_vertices, const float raio, const float height, const vec3& base) 
{
    vec3* vertices = new vec3[num_vertices];
    float angle = 0;
    int iterations = num_vertices / 2;
    float x, y;
    
    for (int i = 0; i < iterations * 2; i+=2) 
    {
        x = (cosf(angle) * raio) + base.x;
        y = (sinf(angle) * raio) + base.y;
        
        vertices[i].x = x;
        vertices[i].y = y;
        vertices[i].z = base.z;
        
        vertices[i+1].x = x;
        vertices[i+1].y = y;
        vertices[i+1].z = height + base.z;
    
        angle += (2.0f * T_PI) / (iterations - 1.0f);
    }

    return vertices;
}

vec3 vec3::normalized()
{
    if (magnitude() == 0.0f)
        return { 0.0f };
    return { x / magnitude(), y / magnitude(), z / magnitude() };
}

float vec3::magnitude()
{
    return sqrtf(x * x + y * y + z * z);
}

vec3 vec3::operator*(float mult)
{
    return { x * mult, y * mult, z * mult };
}

void vec3::operator+=(const vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

std::ostream& operator<<(std::ostream& os, const vec3& v)
{
    os << "( " << v.x << ", " << v.y << ", " << v.z << " )" << std::endl;
    return os;
}
