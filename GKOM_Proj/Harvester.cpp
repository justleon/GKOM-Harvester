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


}


void Harvester::DrawHarvester()
{
	
}
