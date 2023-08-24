#include "Shader.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
	:m_RendererID(0), m_FilePath(filepath)
{
	ShaderProgramSource source = ParseFile(filepath);
	m_RendererID = CreateShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

ShaderProgramSource Shader::ParseFile(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		} 
		else
		{
			ss[(int)type] << line << "\n";
		}
		
	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		const char* message = (char*)alloca(length * sizeof(char));
		std::cout << " Failed to compile " << (type == GL_VERTEX_SHADER ? " vertex " : "fragment ") << "shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteProgram(id);
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int shader = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return shader;
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}