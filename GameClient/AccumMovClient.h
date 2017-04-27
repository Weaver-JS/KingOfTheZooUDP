#pragma once
#include <iostream>
#include <vector>
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
class AccumMovClient
{
	std::vector<movement> movementList;
public:
	AccumMovClient();
	~AccumMovClient();
	void addMovement(movKey mov);
	std::vector<movement> & getMovementList();
};

