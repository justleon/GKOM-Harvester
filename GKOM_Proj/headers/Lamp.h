#pragma once

#include "ObjectPrimitive.h"

class Lamp : public ObjectPrimitive
{
private:
	GLfloat size;

public:
	Lamp(GLfloat size, Transformation transMat);

	void draw(ShaderProgram shader) override;

	std::unique_ptr<VertexBuffer> initVertices();
	std::unique_ptr<IndexBuffer> initIndices();
};