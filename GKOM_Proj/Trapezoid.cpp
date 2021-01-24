#include <iostream>
#include "headers/Trapezoid.h"

Trapezoid::Trapezoid(GLfloat size, Transformation transMat, GLuint texID)
	: ObjectPrimitive(transMat, texID), size(size)
{
	if (size <= 0.0f)
		std::cout << "Dimension can't be <= 0!" << std::endl;
	else
	{
		this->size = size;
		initObject();
	}
}

std::unique_ptr<VertexBuffer> Trapezoid::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[] = {
		// coordinates							// color			// texture
		 0.8f*halfSize,  halfSize,  halfSize,  1.0f,  1.0f,  1.0f,	1.0f,  0.0f,
		-0.8f*halfSize,  halfSize,  halfSize, -1.0f,  1.0f,  1.0f,	0.0f,  0.0f,
		-1.2f*halfSize, -halfSize,  halfSize, -1.0f, -1.0f,  1.0f,	0.0f,  1.0f,
		 1.2f*halfSize, -halfSize,  halfSize,  1.0f, -1.0f,  1.0f,	1.0f,  1.0f,

		 0.8f*halfSize,  halfSize, -halfSize,  1.0f,  1.0f, -1.0f,	1.0f,  1.0f,
		-0.8f*halfSize,  halfSize, -halfSize, -1.0f,  1.0f, -1.0f,	0.0f,  1.0f,
		-1.2f*halfSize, -halfSize, -halfSize, -1.0f, -1.0f, -1.0f,	0.0f,  0.0f,
		 1.2f*halfSize, -halfSize, -halfSize,  1.0f, -1.0f, -1.0f,	1.0f,  0.0f,

		 0.8f*halfSize,  halfSize, halfSize,   1.0f,  1.0f,  1.0f,	0.0f,  1.0f,
		 1.2f*halfSize, -halfSize,  halfSize,  1.0f, -1.0f,  1.0f,	0.0f,  0.0f,
		 0.8f*halfSize,  halfSize, -halfSize,  1.0f,  1.0f, -1.0f,	1.0f,  1.0f,
		 1.2f*halfSize, -halfSize, -halfSize,  1.0f, -1.0f, -1.0f,	1.0f,  0.0f,

		-0.8f*halfSize,  halfSize,  halfSize, -1.0f,  1.0f,  1.0f,	1.0f,  1.0f,
		-1.2f*halfSize, -halfSize,  halfSize, -1.0f, -1.0f,  1.0f,	1.0f,  0.0f,
		-0.8f*halfSize,  halfSize, -halfSize, -1.0f,  1.0f, -1.0f,	0.0f,  1.0f,
		-1.2f*halfSize, -halfSize, -halfSize, -1.0f, -1.0f, -1.0f,	0.0f,  0.0f,

	};

	return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
}

std::unique_ptr<IndexBuffer> Trapezoid::initIndices()
{
	GLuint indices[] = {
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			0,1,5,
			0,4,5,

			8,9,10,
			9,10,11,

			2,3,6,
			3,6,7,

			12,13,14,
			13,14,15
	};

	return std::make_unique<IndexBuffer>(indices, sizeof(indices));
}
