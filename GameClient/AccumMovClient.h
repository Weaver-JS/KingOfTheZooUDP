#pragma once
#include <iostream>
#include <vector>
#include "SFML\System.hpp"
#define MOVEMENTBITS 2
enum movKey
{
	LEFT,
	RIGHT
};
struct movement{
		movKey move;
		int idPacket;
};

struct validation
{
	uint16_t movementI;
		int idPacket;
};
class AccumMovClient
{
	sf::Clock movement_clock; 
	std::vector<movement> movementList;
	//std::vector<validation>
public:
	AccumMovClient();
	~AccumMovClient();
	void addMovement(movKey mov);
	std::vector<movement> & getMovementList();
	uint16_t getAccumMovement();
};

