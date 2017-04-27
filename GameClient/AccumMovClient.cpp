#include "AccumMovClient.h"



AccumMovClient::AccumMovClient()
{
	movement_clock.restart();
}


AccumMovClient::~AccumMovClient()
{
	
}

void AccumMovClient::addMovement(movKey mov)
{
	movement m;
	m.idPacket = movementList.size();
	m.move = mov;
	movementList.push_back(m);
}

std::vector<movement>& AccumMovClient::getMovementList()
{
	return movementList;
}

uint16_t AccumMovClient::getAccumMovement()
{
	uint16_t globalPosition = 0;
	for (int i = 0; i < movementList.size(); i++)
	{
		switch (movementList[i].move)
		{

		case LEFT:
		globalPosition -= 1;
		break;

		case RIGHT:
		globalPosition += 1;
		break;
		
		}
	}
}
