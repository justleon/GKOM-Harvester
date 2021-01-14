#pragma once

#include "ObjectPrimitive.h"

class Triangle : public ObjectPrimitive
{
	GLfloat size;

public:
	//x, y, z, transformation
	Triangle(GLfloat size, Transformation transMat);

	std::unique_ptr<VertexBuffer> initVertices();
	std::unique_ptr<IndexBuffer> initIndices();
};