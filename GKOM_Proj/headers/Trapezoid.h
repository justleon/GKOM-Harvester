#pragma once

#include "ObjectPrimitive.h"

class Trapezoid : public ObjectPrimitive
{
	GLfloat size;

public:
	//x, y, z, transformation
	Trapezoid(GLfloat size, Transformation transMat, GLuint texID);

	std::unique_ptr<VertexBuffer> initVertices();
	std::unique_ptr<IndexBuffer> initIndices();
};

