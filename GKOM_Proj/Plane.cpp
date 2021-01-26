#include "headers/Plane.h"

Plane::Plane(GLfloat size, Transformation transMat, GLuint texID)
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

std::unique_ptr<VertexBuffer> Plane::initVertices()
{
	GLfloat halfSize = size / 2.0f;
	GLfloat vertices[] = {
		// coordinates						// normals			// texture
		 halfSize,  halfSize,  halfSize,   0.0f,  1.0f,  -1.0f,	1000.0f,  0.0f,
		-halfSize,  halfSize,  halfSize,   0.0f,  1.0f,  0.0f,	0.0f,   0.0f,
		-halfSize, -halfSize,  halfSize,   0.0f,  1.0f,  0.0f,	0.0f,  1000.0f,
		 halfSize, -halfSize,  halfSize,   0.0f,  1.0f,  0.0f,	1000.0f, 1000.0f
	};

	return std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
}

std::unique_ptr<IndexBuffer> Plane::initIndices()
{
	GLuint indices[] = {
			0, 1, 2,
			0, 2, 3
	};

	return std::make_unique<IndexBuffer>(indices, sizeof(indices));
}