#include <iostream>
#include "headers/Pyramid.h"

Pyramid::Pyramid(GLfloat size, Transformation transMat)
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

std::unique_ptr<VertexBuffer> Pyramid::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[] = {
		// coordinates						// color			// texture
		0.0f,		halfSize,	0,			0.0f, 0.0f, 1.0f,	0.5f,  1.0f, //F

		-halfSize,	-halfSize,  -halfSize,	1.0f, 0.0f, 0.0f,	0.0f,  0.0f, //D 
		halfSize,	-halfSize,  -halfSize,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f, //E
		
		halfSize,	-halfSize,  -halfSize,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f, //E
		halfSize,	-halfSize,	halfSize,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f, //B

		halfSize,	-halfSize,	halfSize,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f, //B
		-halfSize,	-halfSize,	halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f, //C

		-halfSize,	-halfSize,	halfSize,	1.0f, 0.0f, 0.0f,	0.0f,  0.0f, //C
		-halfSize, -halfSize,  -halfSize,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f, //D

		-halfSize,	-halfSize,	halfSize,	1.0f, 0.0f, 0.0f,	0.0f,  0.0f, //C
		halfSize,	-halfSize,	halfSize,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f, //B
		halfSize,  -halfSize,  -halfSize,	0.0f, 1.0f, 0.0f,	1.0f,  1.0f, //E
		-halfSize, -halfSize,  -halfSize,	1.0f, 0.0f, 0.0f,	0.0f,  1.0f, //D
	};

	return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
}

std::unique_ptr<IndexBuffer> Pyramid::initIndices()
{
	GLuint indices[] = {
			1, 2, 0,
			3, 4, 0,
			5, 6, 0,
			7, 8, 0,
			//
			9, 10, 12,
			10, 11, 12,
	};

	return std::make_unique<IndexBuffer>(indices, sizeof(indices));
}