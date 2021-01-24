#include "headers/ObjectPrimitive.h"

ObjectPrimitive::ObjectPrimitive(Transformation transformMatrix, GLuint texID)
	: transform(transformMatrix), textureID(texID)
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
	useModel();
	shader.Use();

	shader.setUniformMat4("model", modelMat);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	//shader.setUniformInt("material.tex", textureID);

	vertexArray->Bind();
	indexBuffer->Bind();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
	vertexArray->Unbind();
	indexBuffer->Unbind();
}

void ObjectPrimitive::useModel()
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

void ObjectPrimitive::translateWorld(const glm::vec3& vec)
{
	modelMat = glm::translate(modelMat, vec) * modelMat;
}

void ObjectPrimitive::rotateWorld(GLfloat angle, const glm::vec3& axis)
{
	modelMat = glm::rotate(modelMat, glm::radians(angle), axis) * modelMat;
}

void ObjectPrimitive::resizeWorld(const glm::vec3& vec)
{
	modelMat = glm::scale(modelMat, vec) * modelMat;
}