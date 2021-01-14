#pragma once

#include "ObjectPrimitive.h"

class Wheel : public ObjectPrimitive
{
	GLfloat size;

public:

	Wheel(GLfloat size, Transformation transMat);

	std::unique_ptr<VertexBuffer> initVertices();
	std::unique_ptr<IndexBuffer> initIndices();
};

