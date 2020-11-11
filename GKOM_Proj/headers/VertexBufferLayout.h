#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>

struct BufferElement
{
	GLuint type;
	GLuint count;
	unsigned char norm;

	BufferElement(GLuint type, GLuint count, GLboolean norm)
	{
		this->type = type;
		this->count = count;
		this->norm = norm;
	}

	static unsigned int GLGetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:
				return sizeof(GLfloat);
			case GL_UNSIGNED_INT:
				return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:
				return sizeof(GLbyte);
			default:
				return 0;
		}
	}
};

class VertexBufferLayout
{
	std::vector<BufferElement> arrayElements;
	GLuint size;

	void push(GLuint type, GLuint count, unsigned char norm);

public:
	VertexBufferLayout();

	inline const std::vector<BufferElement> getElements() const { return arrayElements; }
	inline const GLuint getSize() const { return size; }

	void addFloat(GLuint count);
	void addUInt(GLuint count);
	void addUByte(GLuint count);
};