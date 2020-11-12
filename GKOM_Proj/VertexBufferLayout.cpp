#include "headers/VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout() : size(0)
{
}

void VertexBufferLayout::push(GLuint type, GLuint count, unsigned char norm)
{
	arrayElements.push_back( { type, count, norm } );
	size += count * BufferElement::GLGetSizeOfType(type);
}

void VertexBufferLayout::addFloat(GLuint count)
{
	push(GL_FLOAT, count, GL_FALSE);
}

void VertexBufferLayout::addUInt(GLuint count)
{
	push(GL_UNSIGNED_INT, count, GL_FALSE);
}

void VertexBufferLayout::addUByte(GLuint count)
{
	push(GL_UNSIGNED_BYTE, count, GL_TRUE);
}