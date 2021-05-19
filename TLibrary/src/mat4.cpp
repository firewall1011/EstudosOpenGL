#include "mat4.h"

float mat4::GetElementAt(unsigned int i, unsigned int j) const
{
	return m[i * 4 + j];
}

float* mat4::At(unsigned int i, unsigned int j)
{
	return &m[i * 4 + j];
}

mat4 mat4::operator*(const mat4& other) const
{
	mat4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				*result.At(i, j) += GetElementAt(i, k) * other.GetElementAt(k, j);
			}
		}
	}
	return result;
}

mat4 mat4::Identity()
{
	return mat4
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

mat4 mat4::translate(const vec3& v)
{
	return 
	{
		1.0f, 0.0f, 0.0f, v.x,
		0.0f, 1.0f, 0.0f, v.y,
		0.0f, 0.0f, 1.0f, v.z,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

mat4 mat4::rotateX(float angle)
{
	return
	{
		1.0f,		 0.0f,		   0.0f, 0.0f,
		0.0f, cosf(angle), -sinf(angle), 0.0f,
		0.0f, sinf(angle),	cosf(angle), 0.0f,
		0.0f,		 0.0f,		   0.0f, 1.0f
	};
}

mat4 mat4::rotateY(float angle)
{
	return
	{
		 cosf(angle), 0.0f, sinf(angle), 0.0f,
		 	    0.0f, 1.0f,		  0.0f, 0.0f,
		-sinf(angle), 0.0f, cosf(angle), 0.0f,
		 	    0.0f, 0.0f,		  0.0f, 1.0f
	};
}

mat4 mat4::rotateZ(float angle)
{
	return
	{
		cosf(angle), -sinf(angle), 0.0f, 0.0f,
		sinf(angle),  cosf(angle), 0.0f, 0.0f,
			   0.0f,	 	 0.0f, 1.0f, 0.0f,
			   0.0f,	 	 0.0f, 0.0f, 1.0f
	};
}


mat4 mat4::rotate(const vec3& v) 
{
	return rotateX(v.x) * rotateY(v.y) * rotateZ(v.z);
}

mat4 mat4::scale(float s)
{
	return mat4
	{
		   s, 0.0f, 0.0f, 0.0f,
		0.0f,    s, 0.0f, 0.0f,
		0.0f, 0.0f,    s, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

mat4 mat4::scale(const vec3& s)
{
	return mat4
	{
		s.x, 0.0f, 0.0f, 0.0f,
		0.0f, s.y, 0.0f, 0.0f,
		0.0f, 0.0f, s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

std::ostream& operator<<(std::ostream& os, const mat4& m) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			os << m.GetElementAt(i, j) << ", ";
		os << m.GetElementAt(i, 3) << std::endl;
	}
	return os;
}