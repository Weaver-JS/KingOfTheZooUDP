#include "PlayerInfo.h"
#include <random>


PlayerInfo::PlayerInfo(sf::IpAddress & ip, unsigned short & port)
{
	player_ip = ip;
	player_port = port;
}

PlayerInfo::~PlayerInfo()
{
}

const sf::IpAddress & PlayerInfo::getPlayerIP()
{
	return player_ip;
}

const unsigned short & PlayerInfo::getPlayerPort()
{
	return player_port;
}

void PlayerInfo::setPlayerIP(sf::IpAddress & ip)
{
	player_ip = ip;
}

void PlayerInfo::setPlayerPort(unsigned short & port)
{
	player_port = port;
}

void PlayerInfo::setPlayerID(uint16_t & id)
{
	playerID = id;
}

uint16_t & PlayerInfo::getX()
{
	return x;
}

uint16_t & PlayerInfo::getY()
{
	// TODO: insert return statement here
	return y;
}

uint16_t & PlayerInfo::getPlayerID()
{
	return playerID;
}
