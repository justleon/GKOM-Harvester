#include "headers/ObjectCollection.h"

void ObjectCollection::addObject(std::shared_ptr<Object> obj)
{
	objects.push_back(obj);
}

void ObjectCollection::draw(ShaderProgram shader)
{
	for (auto& object : objects) {
		object->draw(shader);
	}
}

void ObjectCollection::translate(const glm::vec3& vec)
{
	for (auto& object : objects) {
		object->translate(vec);
	}
}

void ObjectCollection::rotate(GLfloat angle, const glm::vec3& axis)
{
	for (auto& object : objects) {
		object->rotate(angle, axis);
	}
}

void ObjectCollection::resize(const glm::vec3& vec)
{
	for (auto& object : objects) {
		object->resize(vec);
	}
}

void ObjectCollection::translateWorld(const glm::vec3& vec)
{
	for (auto& object : objects) {
		object->translateWorld(vec);
	}
}

void ObjectCollection::rotateWorld(GLfloat angle, const glm::vec3& axis)
{
	for (auto& object : objects) {
		object->rotateWorld(angle, axis);
	}
}

void ObjectCollection::resizeWorld(const glm::vec3& vec)
{
	for (auto& object : objects) {
		object->resizeWorld(vec);
	}
}