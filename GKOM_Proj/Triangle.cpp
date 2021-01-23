#include <iostream>
#include "headers/Triangle.h"

Triangle::Triangle(GLfloat size, Transformation transMat)
	: ObjectPrimitive(transMat), size(size)
{
	if (size <= 0.0f)
		std::cout << "Dimension can't be <= 0!" << std::endl;
	else
	{
		this->size = size;
		initObject();
	}
}

std::unique_ptr<VertexBuffer> Triangle::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[] = {
		// coordinates										// normals			// texture
		 0.8f * halfSize,  halfSize,			halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		 0.8f * halfSize, -halfSize,			halfSize,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
		-0.8f * halfSize,  0.001f * halfSize,	halfSize,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

		 0.8f * halfSize,  halfSize,			-halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		 0.8f * halfSize, -halfSize,			-halfSize,	0.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		-0.8f * halfSize,  0.001f * -halfSize,	-halfSize,	1.0f, 0.0f, 1.0f,	1.0f,  0.0f,
	};

	return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
}

std::unique_ptr<IndexBuffer> Triangle::initIndices()
{
	GLuint indices[] = {
			0,1,2,

			3,4,5,

			0,2,3,
			2,3,5,

			0,1,3,
			1,3,4,

			1,2,4,
			2,4,5
	};

	return std::make_unique<IndexBuffer>(indices, sizeof(indices));
}
