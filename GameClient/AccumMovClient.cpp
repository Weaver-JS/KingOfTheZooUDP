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
	m.idPacket = movementList.size();
	m.move = mov;
	movementList.push_back(m);
}

std::vector<movement>& AccumMovClient::getMovementList()
{
	return movementList;
}
