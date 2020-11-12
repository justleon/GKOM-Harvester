#include "headers/VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &arrayId);
	glBindVertexArray(arrayId);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &arrayId);
}

void VertexArray::LinkBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
	Bind();
	vertexBuffer.Bind();
	const std::vector<BufferElement> elements = layout.getElements();
	GLuint offset = 0;
	for (GLuint i = 0; i < elements.size(); i++)
	{
		const BufferElement el = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, el.count, el.type, el.norm, layout.getSize(), (GLvoid*)(offset));
		offset += el.count * BufferElement::GLGetSizeOfType(el.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(arrayId);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}