#pragma once

#include "ObjectPrimitive.h"

class Sphere : public ObjectPrimitive
{
	GLfloat size;

public:

	Sphere(GLfloat size, Transformation transMat, GLuint texID);

	std::unique_ptr<VertexBuffer> initVertices();
	std::unique_ptr<IndexBuffer> initIndices();
};

