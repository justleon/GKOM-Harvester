#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "ShaderProgram.h"

class Object
{
protected:
	Object() {};
	virtual void draw(ShaderProgram shader) = 0;

	virtual void translate(const glm::vec3& vec) = 0;
	virtual void rotate(GLfloat angle, const glm::vec3& axis) = 0;
	virtual void resize(const glm::vec3& vec) = 0;
};