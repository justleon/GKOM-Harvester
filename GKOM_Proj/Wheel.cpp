#include <iostream>
#include "headers\Wheel.h"

Wheel::Wheel(GLfloat size, Transformation transMat, GLuint texID)
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

std::unique_ptr<VertexBuffer> Wheel::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[] = {
		// coordinates														// color			// texture
		0.0f,		0.0f,	0.5f*halfSize,									0.0f, 0.0f, 1.0f,	1.0f,  1.0f, //sphere north pole

		0.0f,		0.5f * halfSize,	0.6f * halfSize,					1.0f, 1.0f, 1.0f,	0.0f,  1.0f, //second ring above center
		0.25f * halfSize,	0.433f * halfSize,  0.6f * halfSize,			1.0f, 1.0f, 1.0f,	1.0f,  1.0f,
		0.433f * halfSize,	0.25f * halfSize,  0.6f * halfSize,				1.0f, 1.0f, 1.0f,	0.0f,  1.0f,
		0.5f * halfSize,	0.0f,  0.6f * halfSize,							1.0f, 0.0f, 1.0f,	1.0f,  1.0f,
		0.433f * halfSize,	-0.25f * halfSize,	0.6f * halfSize,			1.0f, -1.0f, 1.0f,	0.0f,  1.0f,
		0.25f * halfSize,	-0.433f * halfSize,	0.6f * halfSize,			1.0f, -1.0f, 1.0f,	1.0f,  1.0f,
		0.0f,	-0.5f * halfSize,	0.6f * halfSize,						0.0f, -1.0f, 1.0f,	0.0f,  1.0f,
		-0.25f * halfSize,	-0.433f * halfSize,	0.6f * halfSize,			-1.0f, -1.0f, 1.0f,	1.0f,  1.0f,
		-0.433f * halfSize, -0.25f * halfSize,  0.6f * halfSize,			-1.0f, -1.0f, 1.0f,	0.0f,  1.0f,
		-0.5f * halfSize,	0.0f,	0.6f * halfSize,						-1.0f, 0.0f, 1.0f,	1.0f,  1.0f,
		-0.433f * halfSize,	0.25f * halfSize,	0.6f * halfSize,			-1.0f, 1.0f, 1.0f,	0.0f,  1.0f,
		-0.25f * halfSize,  0.433f * halfSize,  0.6f * halfSize,			-1.0f, 1.0f, 1.0f,	1.0f,  1.0f,

		0.0f,		0.866f * halfSize,	0.5f * halfSize,					0.0f, 1.0f, 1.0f,	0.0f,  0.0f, //first ring above center
		0.433f * halfSize,	0.75f * halfSize,  0.5f * halfSize,				1.0f, 1.0f, 1.0f,	1.0f,  0.0f,
		0.75f * halfSize,	0.433f * halfSize,  0.5f * halfSize,			1.0f, 1.0f, 1.0f,	0.0f,  0.0f,
		0.866f * halfSize,	0.0f,  0.5f * halfSize,							1.0f, 0.0f, 1.0f,	1.0f,  0.0f,
		0.75f * halfSize,	-0.433f * halfSize,	0.5f * halfSize,			1.0f, -1.0f, 1.0f,	0.0f,  0.0f,
		0.433f * halfSize,	-0.75f * halfSize,	0.5f * halfSize,			1.0f, -1.0f, 1.0f,	1.0f,  0.0f,
		0.0f,	-0.866f * halfSize,	0.5f * halfSize,						0.0f, -1.0f, 1.0f,	0.0f,  0.0f,
		-0.433f * halfSize,	-0.75f * halfSize,	0.5f * halfSize,			-1.0f, -1.0f, 1.0f,	1.0f,  0.0f,
		-0.75f * halfSize, -0.433f * halfSize,  0.5f * halfSize,			-1.0f, -1.0f, 1.0f,	0.0f,  0.0f,
		-0.866f * halfSize,	0.0f,	0.5f * halfSize,						-1.0f, 0.0f, 1.0f,	1.0f,  0.0f,
		-0.75f * halfSize,	0.433f * halfSize,	0.5f * halfSize,			-1.0f, 1.0f, 1.0f,	0.0f,  0.0f,
		-0.433f * halfSize,  0.75f * halfSize,  0.5f * halfSize,			-1.0f, 1.0f, 1.0f,	1.0f,  0.0f,

		0.0f,		halfSize,	0.0f,										0.0f, 1.0f, 0.0f,	0.0f,  1.0f, // center ring
		0.5f * halfSize,	0.866f * halfSize,  0.0f,						1.0f, 1.0f, 0.0f,	1.0f,  1.0f,
		0.866f * halfSize,	0.5f * halfSize,  0.0f,							1.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		halfSize,	0.0f,  0.0f,											1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		0.866f * halfSize,	-0.5f * halfSize,	0.0f,						1.0f, -1.0f, 0.0f,	0.0f,  1.0f,
		0.5f * halfSize,	-0.866f * halfSize,	0.0f,						1.0f, -1.0f, 0.0f,	1.0f,  1.0f,
		0.0f,	-halfSize,	0.0f,											0.0f, -1.0f, 0.0f,	0.0f,  1.0f,
		-0.5f * halfSize,	-0.866f * halfSize,	0.0f,						-1.0f, -1.0f, 0.0f,	1.0f,  1.0f,
		-0.866f * halfSize, -0.5f * halfSize,  0.0f,						-1.0f, -1.0f, 0.0f,	0.0f,  1.0f,
		-halfSize,	0.0f,	0.0f,											-1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
		-0.866f * halfSize,	0.5f * halfSize,	0.0f,						-1.0f, 1.0f, 0.0f,	0.0f,  1.0f,
		-0.5f * halfSize,  0.866f * halfSize,  0.0f,						-1.0f, 1.0f, 0.0f,	1.0f,  1.0f,

		0.0f,		0.866f * halfSize,	-0.5f * halfSize,					0.0f, 1.0f, -1.0f,	0.0f,  0.0f, //first ring below center
		0.433f * halfSize,	0.75f * halfSize,  -0.5f * halfSize,			1.0f, 1.0f, -1.0f,	1.0f,  0.0f,
		0.75f * halfSize,	0.433f * halfSize,  -0.5f * halfSize,			1.0f, 1.0f, -1.0f,	0.0f,  0.0f,
		0.866f * halfSize,	0.0f,  -0.5f * halfSize,						1.0f, 0.0f, -1.0f,	1.0f,  0.0f,
		0.75f * halfSize,	-0.433f * halfSize,	-0.5f * halfSize,			1.0f, -1.0f, -1.0f,	0.0f,  0.0f,
		0.433f * halfSize,	-0.75f * halfSize,	-0.5f * halfSize,			1.0f, -1.0f, -1.0f,	1.0f,  0.0f,
		0.0f,	-0.866f * halfSize,	-0.5f * halfSize,						0.0f, -1.0f, -1.0f,	0.0f,  0.0f,
		-0.433f * halfSize,	-0.75f * halfSize,	-0.5f * halfSize,		   -1.0f, -1.0f, -1.0f,	1.0f,  0.0f,
		-0.75f * halfSize, -0.433f * halfSize,  -0.5f * halfSize,		   -1.0f, -1.0f, -1.0f,	0.0f,  0.0f,
		-0.866f * halfSize,	0.0f,	-0.5f * halfSize,					   -1.0f, 0.0f, -1.0f,	1.0f,  0.0f,
		-0.75f * halfSize,	0.433f * halfSize,	-0.5f * halfSize,		   -1.0f, 1.0f, -1.0f,	0.0f,  0.0f,
		-0.433f * halfSize,  0.75f * halfSize,  -0.5f * halfSize,		   -1.0f, 1.0f, -1.0f,	1.0f,  0.0f,

		0.0f,		0.5f * halfSize,	-0.6f * halfSize,					0.0f, 1.0f, -1.0f,	0.0f,  1.0f, //second ring below center
		0.25f * halfSize,	0.433f * halfSize,  -0.6f * halfSize,			1.0f, 1.0f, -1.0f,	1.0f,  1.0f,
		0.433f * halfSize,	0.25f * halfSize,  -0.6f * halfSize,			1.0f, 1.0f, -1.0f,	0.0f,  1.0f,
		0.5f * halfSize,	0.0f,  -0.6f * halfSize,						1.0f, 0.0f, -1.0f,	1.0f,  1.0f,
		0.433f * halfSize,	-0.25f * halfSize,	-0.6f * halfSize,			1.0f, -1.0f, -1.0f,	0.0f,  1.0f,
		0.25f * halfSize,	-0.433f * halfSize,	-0.6f * halfSize,			1.0f, -1.0f, -1.0f,	1.0f,  1.0f,
		0.0f,	-0.5f * halfSize,	-0.6f * halfSize,						0.0f, -1.0f, -1.0f,	0.0f,  1.0f,
		-0.25f * halfSize,	-0.433f * halfSize,	-0.6f * halfSize,		   -1.0f, -1.0f, -1.0f,	1.0f,  1.0f,
		-0.433f * halfSize, -0.25f * halfSize,  -0.6f * halfSize,		   -1.0f, -1.0f, -1.0f,	0.0f,  1.0f,
		-0.5f * halfSize,	0.0f,	-0.6f * halfSize,						-1.0f, 0.0f, -1.0f,	1.0f,  1.0f,
		-0.433f * halfSize,	0.25f * halfSize,	-0.6f * halfSize,			-1.0f, 1.0f, -1.0f,	0.0f,  1.0f,
		-0.25f * halfSize,  0.433f * halfSize,  -0.6f * halfSize,			-1.0f, 1.0f, -1.0f,	1.0f,  1.0f,

		0.0f,		0.5f * halfSize,	-0.5f * halfSize,					0.0f, 1.0f, -1.0f,	0.0f,  0.0f, //tyre rim rear
		0.25f * halfSize,	0.433f * halfSize,  -0.5f * halfSize,			1.0f, 1.0f, -1.0f,	1.0f,  0.0f,
		0.433f * halfSize,	0.25f * halfSize,  -0.5f * halfSize,			1.0f, 1.0f, -1.0f,	0.0f,  0.0f,
		0.5f * halfSize,	0.0f,  -0.5f * halfSize,						1.0f, 0.0f, -1.0f,	1.0f,  0.0f,
		0.433f * halfSize,	-0.25f * halfSize,	-0.5f * halfSize,			1.0f, -1.0f, -1.0f,	0.0f,  0.0f,
		0.25f * halfSize,	-0.433f * halfSize,	-0.5f * halfSize,			1.0f, -1.0f, -1.0f,	1.0f,  0.0f,
		0.0f,	-0.5f * halfSize,	-0.5f * halfSize,						0.0f, -1.0f, -1.0f,	0.0f,  0.0f,
		-0.25f * halfSize,	-0.433f * halfSize,	-0.5f * halfSize,		   -1.0f, -1.0f, -1.0f,	1.0f,  0.0f,
		-0.433f * halfSize, -0.25f * halfSize,  -0.5f * halfSize,		   -1.0f, -1.0f, -1.0f,	0.0f,  0.0f,
		-0.5f * halfSize,	0.0f,	-0.5f * halfSize,					   -1.0f, 0.0f, -1.0f,	1.0f,  0.0f,
		-0.433f * halfSize,	0.25f * halfSize,	-0.5f * halfSize,		   -1.0f, 1.0f, -1.0f,	0.0f,  0.0f,
		-0.25f * halfSize,  0.433f * halfSize,  -0.5f * halfSize,		   -1.0f, 1.0f, -1.0f,	1.0f,  0.0f,

		0.0f,		0.0f,	-0.5f*halfSize,									0.0f, 0.0f, -1.0f,	1.0f,  1.0f, //sphere south pole

		0.0f,		0.5f * halfSize,	0.5f * halfSize,					0.0f, 1.0f, 1.0f,	0.0f,  0.0f, //tyre rim front
		0.25f * halfSize,	0.433f * halfSize,  0.5f * halfSize,			1.0f, 1.0f, 1.0f,	1.0f,  0.0f,
		0.433f * halfSize,	0.25f * halfSize,  0.5f * halfSize,				1.0f, 1.0f, 1.0f,	0.0f,  0.0f,
		0.5f * halfSize,	0.0f,  0.5f * halfSize,							1.0f, 0.0f, 1.0f,	1.0f,  0.0f,
		0.433f * halfSize,	-0.25f * halfSize,	0.5f * halfSize,			1.0f, -1.0f, 1.0f,	0.0f,  0.0f,
		0.25f * halfSize,	-0.433f * halfSize,	0.5f * halfSize,			1.0f, -1.0f, 1.0f,	1.0f,  0.0f,
		0.0f,	-0.5f * halfSize,	0.5f * halfSize,						0.0f, -1.0f, 1.0f,	0.0f,  0.0f,
		-0.25f * halfSize,	-0.433f * halfSize,	0.5f * halfSize,			-1.0f, -1.0f, 1.0f,	1.0f,  0.0f,
		-0.433f * halfSize, -0.25f * halfSize,  0.5f * halfSize,			-1.0f, -1.0f, 1.0f,	0.0f,  0.0f,
		-0.5f * halfSize,	0.0f,	0.5f * halfSize,						-1.0f, 0.0f, 1.0f,	1.0f,  0.0f,
		-0.433f * halfSize,	0.25f * halfSize,	0.5f * halfSize,			-1.0f, 1.0f, 1.0f,	0.0f,  0.0f,
		-0.25f * halfSize,  0.433f * halfSize,  0.5f * halfSize,			-1.0f, 1.0f, 1.0f,	1.0f,  0.0f,
	};

	return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
}

std::unique_ptr<IndexBuffer> Wheel::initIndices()
{
	GLuint indices[] = {
		
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

				25,26,13,
				26,27,14,
				27,28,15,
				28,29,16,
				29,30,17,
				30,31,18,
				31,32,19,
				32,33,20,
				33,34,21,
				34,35,22,
				35,36,23,
				36,25,24,

				13,14,26,
				14,15,27,
				15,16,28,
				16,17,29,
				17,18,30,
				18,19,31,
				19,20,32,
				20,21,33,
				21,22,34,
				22,23,35,
				23,24,36,
				24,13,25,

				25,26,37,
				26,27,38,
				27,28,39,
				28,29,40,
				29,30,41,
				30,31,42,
				31,32,43,
				32,33,44,
				33,34,45,
				34,35,46,
				35,36,47,
				36,25,48,
				
				37,38,26,
				38,39,27,
				39,40,28,
				40,41,29,
				41,42,30,
				42,43,31,
				43,44,32,
				44,45,33,
				45,46,34,
				46,47,35,
				47,48,36,
				48,37,25,

				37,38,49,
				38,39,50,
				39,40,51,
				40,41,52,
				41,42,53,
				42,43,54,
				43,44,55,
				44,45,56,
				45,46,57,
				46,47,58,
				47,48,59,
				48,37,60,
				
				49,50,38,
				50,51,39,
				51,52,40,
				52,53,41,
				53,54,42,
				54,55,43,
				55,56,44,
				56,57,45,
				57,58,46,
				58,59,47,
				59,60,48,
				60,49,37,

					
				49,50,61,
				50,51,62,
				51,52,63,
				52,53,64,
				53,54,65,
				54,55,66,
				55,56,67,
				56,57,68,
				57,58,69,
				58,59,70,
				59,60,71,
				60,49,72,

				61, 62, 50,
				62, 63, 51,
				63, 64, 52,
				64, 65, 53,
				65, 66, 54,
				66, 67, 55,
				67, 68, 56,
				68, 69, 57,
				69, 70, 58,
				70, 71, 59,
				71, 72, 60,
				72, 61, 49,
				

				61,62,73,
				62,63,73,
				63,64,73,
				64,65,73,
				65,66,73,
				66,67,73,
				67,68,73,
				68,69,73,
				69,70,73,
				70,71,73,
				71,72,73,
				72,61,73,

				75, 1, 2,
				76, 2, 3,
				77, 3, 4,
				78, 4, 5,
				79, 5, 6,
				80, 6, 7,
				81, 7, 8,
				82, 8, 9,
				83, 9, 10,
				84, 10, 11,
				85, 11, 12,
				74, 1, 12,

				74, 75, 1,
				75, 76, 2,
				76, 77, 3,
				77, 78, 4,
				78, 79, 5,
				79, 80, 6,
				80, 81, 7,
				81, 82, 8,
				82, 83, 9,
				83, 84, 10,
				84, 85, 11,
				85, 74, 12,
				
				0,74,75,
				75,76,0,
				76,77,0,
				77,78,0,
				78,79,0,
				79,80,0,
				80,81,0,
				81,82,0,
				82,83,0,
				83,84,0,
				84,85,0,
				85,74,0
	};

	return std::make_unique<IndexBuffer>(indices, sizeof(indices));
}
