#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

class IndexBuffer
{
	GLuint bufferId;
	GLuint count;

public:
	IndexBuffer(const GLuint* data, GLuint count);
	~IndexBuffer();

	inline GLuint GetCount() const { return count; }

	void Bind() const;
	void Unbind() const;
};