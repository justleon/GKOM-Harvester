#include <iostream>
#include "headers/Cube.h"

Cube::Cube(GLfloat size, Transformation transMat)
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

std::unique_ptr<VertexBuffer> Cube::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[] = {
		// coordinates						// color			// texture
		 halfSize,  halfSize,  halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-halfSize,  halfSize,  halfSize,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		-halfSize, -halfSize,  halfSize,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
		 halfSize, -halfSize,  halfSize,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

		 halfSize,  halfSize,  halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		 halfSize,  halfSize, -halfSize,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		 halfSize, -halfSize, -halfSize,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
		 halfSize, -halfSize,  halfSize,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

		 halfSize,  halfSize, -halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-halfSize,  halfSize, -halfSize,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		-halfSize, -halfSize, -halfSize,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
		 halfSize, -halfSize, -halfSize,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

		-halfSize,  halfSize, -halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-halfSize,  halfSize,  halfSize,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		-halfSize, -halfSize,  halfSize,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
		-halfSize, -halfSize, -halfSize,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

		 halfSize,  halfSize,  halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		 halfSize,  halfSize, -halfSize,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		-halfSize,  halfSize, -halfSize,	0.0f, 0.0f, 1.0f,	0.0f,  0.0f,
		-halfSize,  halfSize,  halfSize,	1.0f, 0.0f, 1.0f,	0.0f,  1.0f,

		 halfSize, -halfSize,  halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		 halfSize, -halfSize, -halfSize,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		-halfSize, -halfSize, -halfSize,	0.0f, 0.0f, 1.0f,	0.0f,  0.0f,
		-halfSize, -halfSize,  halfSize,	1.0f, 0.0f, 1.0f,	0.0f,  1.0f,
	};

	return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
}

std::unique_ptr<IndexBuffer> Cube::initIndices()
{
	GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23,
	};

	return std::make_unique<IndexBuffer>(indices, sizeof(indices));
}