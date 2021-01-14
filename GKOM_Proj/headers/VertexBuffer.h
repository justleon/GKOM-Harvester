#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class VertexBuffer
{
	GLuint bufferId;

public:
	VertexBuffer(const GLfloat* data, GLuint size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};