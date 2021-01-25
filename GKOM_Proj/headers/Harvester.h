#pragma once
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
	//zmienna okre�la obr�t ca�ego kombajnu
	float angleKompoj;
	

	Harvester(int numOfSides, int numOfMechanisms, float lengthOfWheatPipe);
	void DrawHarvester();

};

