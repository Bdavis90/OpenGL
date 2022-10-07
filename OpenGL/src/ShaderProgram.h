#pragma once
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include <string>
class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	enum ShaderType
	{
		VERTEX,
		FRARMENT,
		PROGRAM
	};

	bool LoadShaders(const char* vsFileName, const char* fsFileName);
	void use();

private:
	std::string fileToString(const std::string& filename);
	void checkCompileErrors(GLuint shader, ShaderType tyep);

	GLuint mHandle;

};

#endif // !SHADER_PROGRAM_H
