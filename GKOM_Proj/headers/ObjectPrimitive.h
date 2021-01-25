#pragma once

#include <memory>
#include <vector>
#include "ObjectInterface.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include <iostream>

struct Transformation
{
	glm::vec3 pos;	//position
	GLfloat angle;	//angle of rotation
	glm::vec3 rot;	//rotation
	glm::vec3 size;	//size

	Transformation(glm::vec3 pos, GLfloat angle, glm::vec3 rot, glm::vec3 size)
	{
		this->pos = pos;
		this->angle = angle;
		this->rot = rot;
		this->size = size;
	}
};

class ObjectPrimitive : public Object
{
protected:
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<VertexBufferLayout> bufferLayout;

	GLuint textureID;
	glm::mat4 modelMat;
	Transformation transform;

	virtual std::unique_ptr<VertexBuffer> initVertices() = 0;
	virtual std::unique_ptr<IndexBuffer> initIndices() = 0;

	void initObject();

public:
	ObjectPrimitive(Transformation transformMatrix, GLuint texID);
	
	void draw(ShaderProgram shader);

	void translate(const glm::vec3& vec);
	void rotate(GLfloat angle, const glm::vec3& axis);
	void resize(const glm::vec3& vec);

	void translateWorld(const glm::vec3& vec);
	void rotateWorld(GLfloat angle, const glm::vec3& axis);
	void resizeWorld(const glm::vec3& vec);
};