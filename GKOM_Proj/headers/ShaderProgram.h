#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
	GLuint programId; 	// The program ID

public:
	// Constructor reads and builds the shader
	ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath);

	// Use the program
	inline void Use() const { glUseProgram(programId); }

	// returns program ID
	inline GLuint get_programID() const { return programId; }

	// set uniform values in this shader program
	void setUniformBool(const std::string &name, bool value) const;
	void setUniformInt(const std::string &name, int value) const;
	void setUniformFloat(const std::string &name, float value) const;
	void setUniformVec3f(const std::string &name, glm::vec3& value) const;
	void setUniformMat4(const std::string &name, glm::mat4& value) const;
};