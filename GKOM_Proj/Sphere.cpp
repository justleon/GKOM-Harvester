#include <iostream>
#include "headers/Sphere.h"
#define SLICE_SEGMENTS 8 
#define SLICES 10

Sphere::Sphere(GLfloat size, Transformation transMat, GLuint texID)
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

std::unique_ptr<VertexBuffer> Sphere::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[8 * (1 + SLICES) * (1 + SLICE_SEGMENTS)];
	int count = 0;

	int vi = 0;
	float x, y, z, xy;
	float radius = 1.0f;
	float nx, ny, nz, lengthInv = 1.0f / radius;
	float s, t;

	float sectorStep = glm::two_pi<float>() / SLICES;
	float stackStep = glm::pi<float>() / SLICE_SEGMENTS;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= SLICE_SEGMENTS; ++i)
	{
		stackAngle = glm::half_pi<float>() - i * stackStep;
		xy = radius * glm::cos(stackAngle);
		z = radius * glm::sin(stackAngle);


		for (int j = 0; j <= SLICES; ++j)
		{
			sectorAngle = j * sectorStep;

			x = xy * glm::cos(sectorAngle);
			y = xy * glm::sin(sectorAngle);
			vertices[vi] = x;
			vertices[vi + 1] = y;
			vertices[vi + 2] = z;


			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			vertices[vi + 3] = nx;
			vertices[vi + 4] = ny;
			vertices[vi + 5] = nz;


			s = (float)j / SLICES;
			t = (float)i / SLICE_SEGMENTS;
			vertices[vi + 6] = s;
			vertices[vi + 7] = t;
			vi += 8;
		}
	}

	auto vertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
	return vertexBuffer;
}

std::unique_ptr<IndexBuffer> Sphere::initIndices()
{
	GLuint indices[3 * 2 * (SLICES) * (SLICE_SEGMENTS - 1)];
	int ii = 0;
	int k1, k2;
	for (int i = 0; i < SLICE_SEGMENTS; ++i)
	{
		k1 = i * (SLICES + 1);
		k2 = k1 + SLICES + 1;

		for (int j = 0; j < SLICES; ++j, ++k1, ++k2)
		{

			if (i != 0)
			{
				indices[ii] = k1;
				indices[ii + 1] = k2;
				indices[ii + 2] = k1 + 1;
				ii += 3;
			}


			if (i != (SLICE_SEGMENTS - 1))
			{
				indices[ii] = k1 + 1;
				indices[ii + 1] = k2;
				indices[ii + 2] = k2 + 1;
				ii += 3;
			}
		}
	}

	auto indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));
	return indexBuffer;
}
