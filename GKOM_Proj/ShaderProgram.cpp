#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#include "headers/ShaderProgram.h"

string ReadShader(const GLchar* shaderPath)
{
	ifstream shader_file;
	shader_file.exceptions(ifstream::badbit);

	shader_file.open(shaderPath);
	stringstream shader_stream;
	shader_stream << shader_file.rdbuf();
	shader_file.close();
	return shader_stream.str();
}

GLuint CompileShader(const GLchar* shaderCode, GLenum shaderType)
{
	GLuint shader_id = glCreateShader(shaderType);
	glShaderSource(shader_id, 1, &shaderCode, nullptr);
	glCompileShader(shader_id);

	// Print compile errors if any
	GLint success = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader_id, sizeof(infoLog), nullptr, infoLog);
		string msg = string("Shader compilation: ") + infoLog;
		throw exception(msg.c_str());
	}
	return shader_id;
}

ShaderProgram::ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// prepare vertex and fragment shaders
	string vertex_code = ReadShader(vertexPath);
	GLuint vertex_id = CompileShader(vertex_code.c_str(), GL_VERTEX_SHADER);

	string fragment_code = ReadShader(fragmentPath);
	GLuint fragment_id = CompileShader(fragment_code.c_str(), GL_FRAGMENT_SHADER);

	// link shader program
	programId = glCreateProgram();
	glAttachShader(programId, vertex_id);
	glAttachShader(programId, fragment_id);
	glLinkProgram(programId);

	// Print linking errors if any
	GLint success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(programId, sizeof(infoLog), nullptr, infoLog);
		string msg = string("Shader program linking: ") + infoLog;
		throw exception(msg.c_str());
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}

void ShaderProgram::setUniformBool(const string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
}

void ShaderProgram::setUniformInt(const string &name, int value) const
{
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

void ShaderProgram::setUniformFloat(const string &name, float value) const
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void ShaderProgram::setUniformMat4(const string &name, glm::mat4 &value) const
{
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

