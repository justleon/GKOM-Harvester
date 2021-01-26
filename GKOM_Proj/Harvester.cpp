#include <iostream>
#include "headers/Harvester.h"
#include "headers/Cube.h"
#include "headers/Pyramid.h"
#include "headers/Cylinder.h"
#include "headers/Sphere.h"
#include "headers/Wheel.h"
#include "headers/Trapezoid.h"
#include "headers/Triangle.h"
#include "headers/ShaderProgram.h"

Harvester::Harvester(int numOfSides, int numOfMechanisms, float lengthOfWPipe)
	: numberOfSidesInMechanism(numOfSides), numberOfMechanisms(numOfMechanisms), lengthOfWheatPipe(lengthOfWPipe)
{
	if (numberOfSidesInMechanism < 3) numberOfSidesInMechanism = 3;
	if (numberOfMechanisms < 2) numberOfMechanisms = 2;
	if (lengthOfWheatPipe < 0.4f) lengthOfWheatPipe = 0.4f;
	distanceBetweenMechanisms = 2.4f / (numberOfMechanisms - 1);
	angle = 360.0 / numberOfSidesInMechanism;
	angleInDegrees = 360.0 / numberOfSidesInMechanism;
	radius = 0.25f;
	angleDiffrence = 0.0f;
	speedOfMechanism = 1.0f;
	//konwersja na radiany
	angle *= (3.1415f / 180.0f);
	sideLengthInMechanism = sqrtf(2 * (1 - cosf(angle))) * radius;
	heightInMechanism = radius * cosf(angle / 2.0f);
	teethDirection = true;
	firstTeethPosition = -1.15f;
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	Front = glm::vec3(-1.0f, 0.0f, 0.0f);
	maxSpeed = 100.0f;
	acceleration = 1.0f;
	rotSpeed = 20.0f;
	speed = 0.0f;
	rotation = 0.0f;
	wheelAngle = 0.0f;
	wheelRotation = 1.0f;
}


void Harvester::accelerate(float deltaTime){
	speed += acceleration * deltaTime;
	if (speed > maxSpeed)
		speed = maxSpeed;
	std::cout << speed << std::endl;
}

void Harvester::decelerate(float a, float deltaTime) {
	speed -= a * deltaTime;
	if (speed < 0.0f)
		speed = 0.0f;
}

void Harvester::move(float deltaTime) {
	if(speed > 0.01f)
		Position += Front * speed * deltaTime;
}

void Harvester::turnRight(float deltaTime) {
	wheelRotation = 30.0f;
	wheelAngle = -90.0f;
	rotation -= rotSpeed * speed * deltaTime;
	updateFront();
}

void Harvester::turnLeft(float deltaTime) {
	wheelRotation = 30.0f;
	wheelAngle = 90.0f;
	rotation += rotSpeed * speed * deltaTime;
	updateFront();
}

void Harvester::updateFront() {
	glm::vec3 front;
	front.x = -cos(glm::radians(rotation));
	front.y = sin(glm::radians(0.0f));
	front.z = sin(glm::radians(rotation));
	Front = glm::normalize(front);
}
