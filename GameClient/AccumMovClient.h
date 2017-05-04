#pragma once
#include <iostream>
#include <vector>
#include "SFML\System.hpp"
#define MOVEMENTBITS 2
#define limitMOV 20
enum movKey
{
	LEFT,
	RIGHT
};
struct movementCheck 
{
	int16_t xCoord;
	int idPacket;

};
struct movement{
		movKey move;
		
};
class AccumMovClient
{
	std::vector<movementCheck> movementCheckList;
	std::vector<movement> movementList;
public:
	AccumMovClient();
	~AccumMovClient();
	void addMovement(movKey mov);
	std::vector<movement> & getMovementList();
	std::vector<movementCheck> & getMovementCheckList();
	int16_t getFinalPosition();
};

