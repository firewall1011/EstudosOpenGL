#pragma once
#include "tvector.h"

struct mat4 {
	float m[16] = {0.0f};

	float GetElementAt(unsigned int i, unsigned int j) const;
	float* At(unsigned int i, unsigned int j);
	mat4 operator*(const mat4& other) const;
	
	static mat4 Identity();
	static mat4 translate(const vec3&);
	static mat4 rotateX(float);
	static mat4 rotateY(float);
	static mat4 rotateZ(float);
	static mat4 rotate(const vec3&);
	static mat4 scale(float);
	static mat4 scale(const vec3&);
};

std::ostream& operator<<(std::ostream& os, const mat4& m);