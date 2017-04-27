#pragma once
#include "SFML\Network.hpp"
#include <iostream>
#include <PlayerInfo.h>
#include <OutputMemoryBitStream.h>
#include <InputMemoryBitStream.h>
#define PORT 50000
#define BINARYPACKETYPELENGTH 4
#define POSITION_BYNARY_LENGTH 10
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define MAX_PLAYER 2
enum PacketType
{
	PT_EMPTY,
	PT_MOV,
	PT_HELLO,
	PT_WELCOME,
	PT_FULL,
	PT_PING,
	PT_PLAYER_POSITION,
	PT_DISCONNECT,
	PT_SHUTDOWN
};
class UDPmanager
{
private:
	std::vector<PlayerInfo*> playerList; //En udp al poseer un socket, lo que guardamos son ip:puerto
	sf::UdpSocket socket;
	bool abort;
	bool isConnected;
	sf::Clock server_clock;
	sf::Clock shutdown_clock;
	sf::Clock ping_clock;

public:
	UDPmanager();
	~UDPmanager();
	void initConnection();

	int  getPlayerID(short  port);
	void readMessage(char*  _message,const size_t & _sizeMessage,sf::IpAddress & ip, unsigned short & port);
	bool sendMessage(char* message, sf::IpAddress & ip, unsigned short & port, uint32_t sizeBuffer);
	void serverDisconnection();
	void throw_command();
	/* 
		
	*/

	
};

