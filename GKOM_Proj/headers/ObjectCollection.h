#pragma once

#include "ObjectInterface.h"
#include "ObjectPrimitive.h"
#include "ShaderProgram.h"
#include <vector>

class ObjectCollection : public Object
{
public:
	ObjectCollection() {}

	void addObject(std::shared_ptr<Object> obj);

	void draw(ShaderProgram shader);

	void translate(const glm::vec3& vec);
	void rotate(GLfloat angle, const glm::vec3& axis);
	void resize(const glm::vec3& vec);

	void translateWorld(const glm::vec3& vec);
	void rotateWorld(GLfloat angle, const glm::vec3& axis);
	void resizeWorld(const glm::vec3& vec);

	std::vector<std::shared_ptr<Object>> objects;
};