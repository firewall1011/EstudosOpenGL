#include "Shader.h"
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

namespace TLibrary {
	Shader::Shader(const std::string& filepath)
	{
		ShaderCode code = ParseShader(filepath);
		_uid = CreateProgram(code);
	}

	Shader::~Shader()
	{
		glDeleteProgram(_uid);
	}

	ShaderCode Shader::ParseShader(const std::string& filepath)
	{
		FILE* file;
		if (fopen_s(&file, filepath.c_str(), "r") != 0)
		{
			/* Three ways to create error message: */
			perror("perror says open failed");
			return {};
		}
		
		char buffer[128];
		std::string code[2];
		int shaderType = -1; // 0 vertex, 1 fragment

		while (fgets(buffer, 128, file) != NULL) {
			std::string line = buffer;
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos)
					shaderType = 0;
				else if (line.find("fragment") != std::string::npos)
					shaderType = 1;
			}
			else {
				if (shaderType == -1)
					continue;
				code[shaderType].append(buffer);
			}
		}

		fclose(file);
		return { code[0], code[1] };
	}

	void Shader::WriteShader(const std::string& filepath, const char* vertex, const char* fragment)
	{
		FILE* file;
		if (fopen_s(&file, filepath.c_str(), "w") != 0)
		{
			/* Three ways to create error message: */
			perror("perror says open failed");
		}
		else {
			fputs("#shader vertex\n", file);
			fputs(vertex, file);
			fputs("#shader fragment\n", file);
			fputs(fragment, file);

			fclose(file);
		}
	}

	unsigned int Shader::CreateProgram(const ShaderCode& shaderCode)
	{
		// Requisitando slot para a GPU para nossos programas Vertex e Fragment Shaders
		GLuint program = glCreateProgram();
		GLuint vertex = CompileShader(shaderCode.vertex, GL_VERTEX_SHADER);
		GLuint fragment = CompileShader(shaderCode.fragment, GL_FRAGMENT_SHADER);

		// Combinando shaders com programa, linkando o programa e validando
		glAttachShader(program, vertex);
		glAttachShader(program, fragment);
		
		glLinkProgram(program);
		/*glValidateProgram(program);*/

		// Deleta .obj da memoria
	/*	glDeleteShader(vertex);
		glDeleteShader(fragment);*/

		return program;
	}

	unsigned int Shader::CompileShader(const std::string& source, const unsigned int shaderType)
	{
		GLuint shader = glCreateShader(shaderType);
		
		const char* src = source.c_str();
		glShaderSource(shader, 1, &src, NULL);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		
		if (isCompiled == GL_FALSE) {

			//descobrindo o tamanho do log de erro
			int infoLength = 512;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

			//recuperando o log de erro e imprimindo na tela
			char *info = new char[infoLength];
			glGetShaderInfoLog(shader, infoLength, NULL, info);

			std::cout << "\nShader compilation error" << std::endl;
			printf("\nerro: %s\n", info);
		}

		return shader;
	}

	void Shader::Bind() const
	{
		glUseProgram(_uid);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	GLuint Shader::GetID()
	{
		return _uid;
	}



}
