#pragma once

#include <memory>
#include "ObjectInterface.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include <iostream>

class ObjectPrimitive : public Object
{
private:
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<VertexBufferLayout> bufferLayout;

	glm::mat4 model;

protected:
	virtual std::unique_ptr<VertexBuffer> initVertices() = 0;
	virtual std::unique_ptr<IndexBuffer> initIndices() = 0;

	void initObject();

public:
	ObjectPrimitive(const GLfloat angle, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& size);
	
	void draw(ShaderProgram shader) override
	{
		model = glm::mat4(1.0f);

		shader.Use();
		shader.setUniformMat4("model", model);

		vertexArray->Bind();
		indexBuffer->Bind();
		glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
		vertexArray->Unbind();
		indexBuffer->Unbind();
	}

	glm::mat4 GetModel() { return model; }

	void translate(const glm::vec3& vec) override
	{
		model = glm::translate(model, vec);
	}

	void rotate(GLfloat angle, const glm::vec3& axis) override
	{
		model = glm::rotate(model, glm::radians(angle), axis);
	}
	
	void resize(const glm::vec3& vec) override
	{
		model = glm::scale(model, vec);
	}
};