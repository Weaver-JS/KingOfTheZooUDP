#include "UDPmanager.h"



UDPmanager::UDPmanager()
{
	notConnected = true;
	socket.setBlocking(true);
	disconnected = false;
	playerList.resize(MAXPLAYERS);

	
}


UDPmanager::~UDPmanager()
{
	socket.unbind();
}

void UDPmanager::initConnection()
{
	
	client_clock.restart();

	while (notConnected)
	{
		if(client_clock.getElapsedTime() + sf::milliseconds(800)  >= sf::seconds(1.0f))
		{
			disconnected = true;
			notConnected = false;
		
		}
		else
		{
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_HELLO, BINARYPACKETYPELENGTH);
		
			if (sendMessage(ombs.GetBufferPtr(), IP, PORT, ombs.GetByteLength()))
			{
				sf::IpAddress senderIP;
				unsigned short senderPort;
				sf::Packet packet;
				sf::Socket::Status status = socket.receive(packet, senderIP, senderPort) ;
				if (status == sf::Socket::Done)
				{
					readMessage((char*)packet.getData(), packet.getDataSize(), senderIP, senderPort);

				}
				else if(status == sf::Socket::Status::Disconnected)
				{
					disconnected = true;
					notConnected = false;
				}
			}
		}

	}


	
}

void UDPmanager::ping(int16_t x, int16_t y, int packetID)
{
	if (x != 0)
	{
		if (playerList[player->getPlayerID()].getX() + x > 0 && playerList[player->getPlayerID()].getX() + x < SCREEN_WIDTH)
		{
			OutputMemoryBitStream ombs_;
			uint16_t id = player->getPlayerID();
			int16_t completedX = x + player->getX();
			int16_t calcX = x;
			int16_t packid = packetID;
			std::cout << "PAQUETE" << std::endl;
			std::cout << "Completed X : " << completedX << std::endl;
			std::cout << "CalcX: " << calcX << std::endl;
			std::cout << "PAQUETE" << std::endl;
			ombs_.Write(PacketType::PT_MOV, BINARYPACKETYPELENGTH);
			ombs_.Write(id, BINARYPACKETYPELENGTH);
			ombs_.Write(packid, POSITION_BYNARY_LENGTH);
			ombs_.Write(calcX, POSITION_BYNARY_LENGTH);
			ombs_.Write(completedX, POSITION_BYNARY_LENGTH);
			ombs_.Write(y, POSITION_BYNARY_LENGTH);
			sendMessage(ombs_.GetBufferPtr(), IP, PORT, ombs_.GetByteLength());
		}
	}



}

void UDPmanager::readMessage(char*  _message, const size_t & _sizeMessage, sf::IpAddress & ip, unsigned short & port)
{
	//ESTA DEMASIADO ENFOCADO A SERVIDOR

	
	InputMemoryBitStream imbs((char*)_message, _sizeMessage * 8);
	PacketType pt = PacketType::PT_EMPTY;
	std::string receivedString;
	imbs.Read(&pt, BINARYPACKETYPELENGTH);

	switch (pt)
	{
	case PT_EMPTY:
		std::cout << "Paquete vacío" << std::endl;
		break;
	case PT_WELCOME:
	{
		uint16_t Id= 0, x = 0, y = 0;
		player = new PlayerInfo(ip,port);
		
		imbs.ReadBits(&Id, BINARYPACKETYPELENGTH);
		imbs.ReadBits(&player->getX() , POSITION_BYNARY_LENGTH);
		imbs.ReadBits(&player->getY() , POSITION_BYNARY_LENGTH);
		player->setPlayerID(Id);
		playerList[Id].getX() = player->getX();
		playerList[Id].getY() = player->getY();
		playerList[Id].setPlayerID(Id);
		
		notConnected = false;
	}
		break;
	case PT_DISCONNECT:
		std::cout << "disconnecting" << std::endl;
		disconnect();
		
		//disconnected = true;
		break;
	case PT_OKMOVE:
	{
		uint16_t Id = 0,x = 0;
		imbs.ReadBits(&Id, BINARYPACKETYPELENGTH);
		imbs.ReadBits(&x, POSITION_BYNARY_LENGTH);
		playerList[Id].getX() = x;
		
		
	}
	break;
	case PT_FULL:
		disconnected = true;
		break;
	case PT_PING:
	{
		imbs.ReadBits(&size, BINARYPACKETYPELENGTH);
		if (playerList.size() != size)
		{
			playerList.resize(size);
		}
		int id = -1;
		imbs.ReadBits(&id, BINARYPACKETYPELENGTH);
		imbs.ReadBits(&playerList[id].getX(), POSITION_BYNARY_LENGTH);
		imbs.ReadBits(&playerList[id].getY(), POSITION_BYNARY_LENGTH);
	
	}
		break;
	case PT_SHUTDOWN:
		std::cout << "SERVER SHUTTING DOWN" << std::endl;
		std::cout << "Cerrando cliente en 3 segundos "  << std::endl;
		disconnected = true;
		
		
		break;
	case PT_PLAYER_POSITION:
	{
		int ID = 1;
		imbs.ReadBits(&ID, BINARYPACKETYPELENGTH);
		imbs.ReadBits(&playerList[ID].getX(), POSITION_BYNARY_LENGTH);
		imbs.ReadBits(&playerList[ID].getY(), POSITION_BYNARY_LENGTH);

		
	}
		break;
	default:
		std::cout << _message << std::endl;

	}

	
}

void UDPmanager::recv()
{
	while (!disconnected)
	{
		
		sf::IpAddress senderIP;
		unsigned short senderPort;
		sf::Packet packet;
		if (socket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
		{
			readMessage((char*)packet.getData(), packet.getDataSize(), senderIP, senderPort);

		}
	//	
		
	}
}
bool UDPmanager::sendMessage(char * message, sf::IpAddress ip, unsigned short port,uint32_t sizeBuffer)
{
	
		sf::Packet packet;
		packet.append(message, sizeBuffer);
		sf::Socket::Status status = socket.send(packet, ip, port);
		if (status == sf::Socket::Done)
		{
			return true;
		}
		else if (status == sf::Socket::Disconnected)
		{
			return false;
		}

		//return (socket.send(packet, ip, port) == sf::Socket::Done) ? true : false;
		client_clock.restart();
	
	
	return true;
	

}

PlayerInfo & UDPmanager::getPlayer()
{
	return *player;
}

PlayerInfo & UDPmanager::getPlayer(int i)
{

		return playerList[i];
	
}

int  UDPmanager::getPlayerSize()
{
	return playerList.size();
}

void UDPmanager::disconnect()
{
	shutdown_Clock.restart();
	while (!disconnected)
	{
		if(shutdown_Clock.getElapsedTime() > sf::seconds(3.0))
		{
			disconnected = true;
		}
		else
		{
		
				OutputMemoryBitStream ombs;
				ombs.Write(PacketType::PT_DISCONNECT, BINARYPACKETYPELENGTH);



				sendMessage(ombs.GetBufferPtr(), IP, PORT, ombs.GetBitLength());
				
				client_clock.restart();
			
		}
	}
	
}

void UDPmanager::sendAccumList(std::vector<movement> & movlist)
{
	for (int i = 0; i < movlist.size(); i++)
	{
		OutputMemoryBitStream ombs_;
		ombs_.Write(PT_MOV, BINARYPACKETYPELENGTH);
		//ombs_.Write(movlist[i], POSITION_BYNARY_LENGTH);
		ombs_.Write(movlist[i].move, MOVEMENTBITS);
		sendMessage(ombs_.GetBufferPtr(), IP, PORT, ombs_.GetBitLength());

	}
}

bool UDPmanager::isDisconnected()
{
	return disconnected;
}

bool UDPmanager::getNotConnected()
{
	return notConnected;
}
