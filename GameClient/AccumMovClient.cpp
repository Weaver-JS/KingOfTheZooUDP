#include "AccumMovClient.h"



AccumMovClient::AccumMovClient()
{
}


AccumMovClient::~AccumMovClient()
{
}

void AccumMovClient::addMovement(movKey mov)
{
	movement m;
	//m.idPacket = movementList.size();
	m.move = mov;
	movementList.push_back(m);
}

std::vector<movement>& AccumMovClient::getMovementList()
{
	return movementList;
}

std::vector<movementCheck>& AccumMovClient::getMovementCheckList()
{
	return movementCheckList;
}

int16_t AccumMovClient::getFinalPosition()
{
	int16_t x = 0;
	
	for (int i = 0; i < movementList.size(); i++)
	{
		if (x < limitMOV)
		{
			switch (movementList[i].move)
			{
			case RIGHT:
				x++;
				break;
			case LEFT:
				x--;
				break;

			}
		}
	}

	movementCheck c;
	c.idPacket = movementCheckList.size();
	c.xCoord = x;
	movementCheckList.push_back(c);
	movementList.clear();

	return x;
}

