#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	unsigned int getId() const { return m_RendererID; }
	void setInt(const std::string& name, int value) const;

private:
	ShaderProgramSource ParseFile(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};