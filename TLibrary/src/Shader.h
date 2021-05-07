#pragma once
#include <string>

namespace TLibrary {

	struct ShaderCode {
		const std::string vertex;
		const std::string fragment;
	};

	class Shader
	{
	private:
		unsigned int _uid;

	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;
		
		unsigned int GetID();

		static ShaderCode ParseShader(const std::string& filepath);
		static void WriteShader(const std::string& filepath, const char* vertex, const char* fragment);
		static unsigned int CreateProgram(const ShaderCode& shaderCode);
		static unsigned int CompileShader(const std::string& source, const unsigned int shaderType);
	};
}


