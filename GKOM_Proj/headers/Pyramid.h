#pragma once

#include "ObjectPrimitive.h"

class Pyramid : public ObjectPrimitive
{
	GLfloat size;

public:
	//x, y, z, transformation
	Pyramid(GLfloat size, Transformation transMat, GLuint texID); 

	std::unique_ptr<VertexBuffer> initVertices();
	std::unique_ptr<IndexBuffer> initIndices();
};