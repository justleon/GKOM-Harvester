#include <iostream>
#include "headers/Cylinder.h"
#define SIDES 38 //does not work for 64 and higher
#define POINT_VALUES 8
#define TEXTURE_RAD_OFFSET 0.005f

Cylinder::Cylinder(GLfloat size, Transformation transMat, GLuint texID) 
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

std::unique_ptr<VertexBuffer> Cylinder::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[] = {
		// coordinates										// color			// texture
		0.0f,		0.0f,	halfSize,						0.0f, 0.0f, 1.0f,	1.0f,  1.0f, //cylinder upper center

		0.0f,		halfSize,	halfSize,					0.0f, 0.0f, 1.0f,	1.0f,  0.0f,
		0.25f * size,	0.432f * size,  halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		0.432f * size,	0.25f * size,  halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		halfSize,	0.0f,  halfSize,						0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		0.432f * size,	-0.25f * size,	halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		0.25f * size,	-0.432f * size,	halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
		0.0f,	-halfSize,	halfSize,						1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-0.25f * size,	-0.432f * size,	halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		-0.432f * size, -0.25f * size,  halfSize,			1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
		-halfSize,	0.0f,	halfSize,						1.0f, 0.0f, 0.0f,	0.0f,  0.0f,
		-0.432f * size,	0.25f * size,	halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
		-0.25f * size,  0.432f * size,  halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  0.0f,

		0.0f,		halfSize,	-halfSize,					0.0f, 0.0f, 1.0f,	1.0f,  1.0f,
		0.25f * size,	0.432f * size,  -halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		0.432f * size,	0.25f * size,  -halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		halfSize,	0.0f,  -halfSize,						0.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		0.432f * size,	-0.25f * size,	-halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		0.25f * size,	-0.432f * size,	-halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		0.0f,	-halfSize,	-halfSize,						1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		-0.25f * size,	-0.432f * size,	-halfSize,			1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		-0.432f * size, -0.25f * size,  -halfSize,			1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		-halfSize,	0.0f,	-halfSize,						1.0f, 0.0f, 0.0f,	0.0f,  1.0f,
		-0.432f * size,	0.25f * size,	-halfSize,			0.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		-0.25f * size,  0.432f * size,  -halfSize,			0.0f, 1.0f, 0.0f,	0.0f,  1.0f,

		0.0f,		0.0f,	-halfSize,						0.0f, 0.0f, 1.0f,	1.0f,  0.0f, //cylinder lower center
	};
	return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
}

std::unique_ptr<IndexBuffer> Cylinder::initIndices()
{
	GLuint indices[] = {
			//upper lid
			0, 1, 2,
			0, 2, 3,
			0, 3, 4,
			0, 4, 5,
			0, 5, 6,
			0, 6, 7,
			0, 7, 8,
			0, 8, 9,
			0, 9, 10,
			0, 10, 11,
			0, 11, 12,
			0, 1, 12,

			//sides
			13, 1, 2,
			14, 2, 3,
			15, 3, 4,
			16, 4, 5,
			17, 5, 6,
			18, 6, 7,
			19, 7, 8,
			20, 8, 9,
			21, 9, 10,
			22, 10, 11,
			23, 11, 12,
			24, 1, 12,

			2, 13, 14,
			3, 14, 15,
			4, 15, 16,
			5, 16, 17,
			6, 17, 18,
			7, 18, 19,
			8, 19, 20,
			9, 20, 21,
			10, 21, 22,
			11, 22, 23,
			12, 23, 24,
			1, 24, 13,

			//lower lid
			25, 13, 14,
			25, 14, 15,
			25, 15, 16,
			25, 16, 17,
			25, 17, 18,
			25, 18, 19,
			25, 19, 20,
			25, 20, 21,
			25, 21, 22,
			25, 22, 23,
			25, 23, 24,
			25, 24, 13,

	};
	return std::make_unique<IndexBuffer>(indices, sizeof(indices));
}