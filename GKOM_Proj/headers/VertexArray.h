#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
	GLuint arrayId;

public:
	VertexArray();
	~VertexArray();

	void LinkBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};