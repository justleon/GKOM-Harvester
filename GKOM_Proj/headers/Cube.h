#pragma once

#include "ObjectPrimitive.h"

class Cube : public ObjectPrimitive
{
public:
	Cube(const GLfloat angle = 0.0f,
		const glm::vec3& pos = { 0.0f, 0.0f, 0.0f },
		const glm::vec3& rot = { 0.0f, 0.0f, 0.0f },
		const glm::vec3& size = { 1.0f, 1.0f, 1.0f }) : ObjectPrimitive(angle, pos, rot, size)
	{
		initObject();
	}

	std::unique_ptr<VertexBuffer> initVertices() override
	{
		GLfloat vertices[] = {
			// coordinates			// color			// texture
			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

			 0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 1.0f,	1.0f,  1.0f,

			 0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	0.0f,  1.0f,

			 0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 0.0f,	1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,	1.0f, 0.0f, 1.0f,	0.0f,  1.0f,
		};

		return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	}

	std::unique_ptr<IndexBuffer> initIndices() override
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
};