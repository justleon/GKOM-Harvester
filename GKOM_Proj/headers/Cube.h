#pragma once

#include "ObjectPrimitive.h"

class Cube : public ObjectPrimitive
{
	GLfloat size;

public:
	//x, y, z, transformation
	Cube(GLfloat size, Transformation transMat, GLuint texID);

	std::unique_ptr<VertexBuffer> initVertices();
	std::unique_ptr<IndexBuffer> initIndices();
};