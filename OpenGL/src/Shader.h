#pragma once
#include "Renderer.h"
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertextSource;
	std::string FragmentSource;
};

class Shader
{
private:
	GLuint m_RendererID;
	std::string m_FilePath;
	// caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	GLuint CompileShader(GLuint type, const std::string& source);
	GLuint GetUniformLocation(const std::string& name);
};
