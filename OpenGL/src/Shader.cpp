#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>



Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertextSource, source.FragmentSource);

}
Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}


ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Create program for our shader
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Attach our shaders to the program
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (!result)
	{
		GLint length = 0;
		// allocate on stack
		GLchar* infoLog = (GLchar*)alloca(length * sizeof(GLchar));
		glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
		std::cerr << "Error! Shader Program linker failed. " << infoLog << std::endl;
	}

	// Delete already linked shaders
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;

}

GLuint Shader::CompileShader(GLuint type, const std::string& source)
{
	//Create vertex shader
	GLuint id = GLCall(glCreateShader(type));
	const GLchar* src = source.c_str();
	// use our vertex shader source code
	GLCall(glShaderSource(id, 1, &src, nullptr));
	// compile our vertex shader
	GLCall(glCompileShader(id));

	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		GLint length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		// allocate on stack
		GLchar* infoLog = (GLchar*)alloca(length * sizeof(GLchar));
		glGetShaderInfoLog(id, length, &length, infoLog);
		std::cerr << "Error! Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "vertex " : "fragment ")
			<< "shader." << std::endl;
		std::cerr << infoLog << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const
{
	GLCall(glUseProgram(0));

}
// Set uniforms
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if(location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	m_UniformLocationCache[name] = location;
	return location;

}