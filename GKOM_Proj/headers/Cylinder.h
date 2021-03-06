#pragma once

#include "ObjectPrimitive.h"

class Cylinder : public ObjectPrimitive
{
	GLfloat size;

public:

	Cylinder(GLfloat size, Transformation transMat, GLuint texID);

	std::unique_ptr<VertexBuffer> initVertices();
	std::unique_ptr<IndexBuffer> initIndices();
};

