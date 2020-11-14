#include "headers/ObjectPrimitive.h"

ObjectPrimitive::ObjectPrimitive(Transformation transformMatrix)
	: transform(transformMatrix)
{
	
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

void ObjectPrimitive::draw(ShaderProgram shader)
{
	transformModel();

	shader.Use();
	shader.setUniformMat4("model", modelMat);

	vertexArray->Bind();
	indexBuffer->Bind();
	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	vertexArray->Unbind();
	indexBuffer->Unbind();
}

void ObjectPrimitive::transformModel()
{
	modelMat = glm::mat4(1.0f);
	translate(transform.pos);
	rotate(transform.angle, transform.rot);
	resize(transform.size);
}

void ObjectPrimitive::translate(const glm::vec3& vec)
{
	modelMat = glm::translate(modelMat, vec);
}

void ObjectPrimitive::rotate(GLfloat angle, const glm::vec3& axis)
{
	modelMat = glm::rotate(modelMat, glm::radians(angle), axis);
}

void ObjectPrimitive::resize(const glm::vec3& vec)
{
	modelMat = glm::scale(modelMat, vec);
}