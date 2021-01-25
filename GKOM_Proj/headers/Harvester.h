#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Harvester
{
public:
	//zmienna okre�la ilo�� bok�w w m��carce
	int numberOfSidesInMechanism;
	//zmienna okre�la ilo�� element�w w m��carce
	int numberOfMechanisms;
	float distanceBetweenMechanisms;
	float angle;
	float angleInDegrees;
	float radius;
	float angleDiffrence;
	float speedOfMechanism;
	float sideLengthInMechanism;
	float heightInMechanism;
	//zmienna okre��a d�ugo�� rury zbo�owej
	float lengthOfWheatPipe;
	//zmienna okre�la kierunek poruszania si� z�b�w
	bool teethDirection;
	float firstTeethPosition;
	glm::vec3 Position;
	glm::vec3 Front;
	float maxSpeed;
	float acceleration;
	float rotSpeed;
	float speed;
	float rotation;

	Harvester(int numOfSides, int numOfMechanisms, float lengthOfWheatPipe);
	void accelerate(float deltaTime);
	void decelerate(float a, float deltaTime);
	void turnRight(float deltaTime);
	void turnLeft(float deltaTime);
	void move(float deltaTime);
	void updateFront();

};

