#pragma once
class Harvester
{
public:
	//zmienna okreœla iloœæ boków w m³ócarce
	int numberOfSidesInMechanism;
	//zmienna okreœla iloœæ elementów w m³ócarce
	int numberOfMechanisms;
	float distanceBetweenMechanisms;
	float angle;
	float angleInDegrees;
	float radius;
	float angleDiffrence;
	float speedOfMechanism;
	float sideLengthInMechanism;
	float heightInMechanism;
	//zmienna okreœ³a d³ugoœæ rury zbo¿owej
	float lengthOfWheatPipe;
	//zmienna okreœla kierunek poruszania siê zêbów
	bool teethDirection;
	float firstTeethPosition;
	//zmienna okreœla obrót ca³ego kombajnu
	float angleKompoj;
	

	Harvester(int numOfSides, int numOfMechanisms, float lengthOfWheatPipe);
	void DrawHarvester();

};

