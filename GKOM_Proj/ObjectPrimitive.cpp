#include "headers/ObjectPrimitive.h"

ObjectPrimitive::ObjectPrimitive(const GLfloat angle, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& size) : model(glm::mat4(1.0f))
{
	translate(pos);
	rotate(angle, rot);
	resize(size);
}

void ObjectPrimitive::initObject()
{
	vertexArray = std::make_unique<VertexArray>();
	bufferLayout = std::make_unique<VertexBufferLayout>();

	vertexBuffer = initVertices();
	indexBuffer = initIndices();

	bufferLayout->addFloat(3);
	bufferLayout->addFloat(3);
	bufferLayout->addFloat(2);

	vertexArray->LinkBuffer(*vertexBuffer, *bufferLayout);
}
