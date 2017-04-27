#include "UDPmanager.h"



UDPmanager::UDPmanager()
{
	abort = false;
	isConnected = true;
	socket.setBlocking(false);
	server_clock.restart();
}


UDPmanager::~UDPmanager()
{
	socket.unbind();
	
}


void UDPmanager::initConnection()
{
	
	sf::Socket::Status status = socket.bind(PORT);
	if (status != sf::Socket::Done)
	{
		std::cout << "No se puede vincular al puerto: " << PORT << std::endl;
	}
	else if (status == sf::Socket::Done)
	{
		std::cout << "Puerto vinculado: " << PORT << std::endl;
		socket.setBlocking(false);
		

	}

	while (isConnected)
	{
		
		
		sf::IpAddress senderIP;
		unsigned short senderPort;
		sf::Packet packet;
	
			if (socket.receive(packet, senderIP, senderPort) == sf::Socket::Done)
			{
				readMessage((char*)packet.getData(), packet.getDataSize(), senderIP, senderPort);
			}
			server_clock.restart();
		
			sf::sleep(sf::milliseconds(200));
	}
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}


void UDPmanager::throw_command()
{
	while (isConnected)
	{
		std::string command = "";
		std::cin >> command;
		if (command != "")
		{
			switch (str2int(command.c_str()))
			{
			case str2int("exit"):
				serverDisconnection();
				break;
			case str2int("EXIT"):
				serverDisconnection();
				break;

			default:
				std::cout << "Wrong command: Availables command are 'exit' or 'EXIT'" << std::endl;
				break;
			}
		}
	}
	
}

int  UDPmanager::getPlayerID(short  port)
{
	for (int i = 0; i < playerList.size(); i++)
	{
		if (playerList[i]->getPlayerPort() == port)
			return i;
	}
	return -1;
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
	case PT_HELLO:
	{
		if (playerList.size() >= MAX_PLAYER)
		{
			std::cout << "Server is full" << std::endl;
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_FULL, BINARYPACKETYPELENGTH);
			sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
			break;
		}
		else
		{
			for (int i = 0; i < playerList.size(); i++)
			{
				if (playerList[i]->getPlayerPort() == port)
				{
					std::cout << "Jugador ya en la lista" << std::endl;
					abort = true;
				}
			}
			if (abort)
			{
				abort = false;
				break;
			}

			std::cout << "JUGADOR ACEPTADO" << ip << " " << port << std::endl;
			PlayerInfo* newPlayer = new PlayerInfo(ip, port);
			playerList.push_back(newPlayer);
			OutputMemoryBitStream ombs;
			ombs.Write(PacketType::PT_WELCOME, BINARYPACKETYPELENGTH);

			int16_t id, x, y;
			switch (playerList.size() - 1)
			{
			case 0:
				x = 0;
				y = SCREEN_HEIGHT;
				break;
			case 1:
				x = SCREEN_WIDTH;
				y = 0;
				break;
			}
			/*x = rand() % SCREEN_WIDTH;
			y = rand() % SCREEN_HEIGHT + 200;*/
			id = playerList.size() - 1;
			
			playerList[playerList.size() - 1]->getX() = x;
			playerList[playerList.size() - 1]->getY() = y;

			
			ombs.Write(id, BINARYPACKETYPELENGTH);
			ombs.Write(x, POSITION_BYNARY_LENGTH);
			ombs.Write(y, POSITION_BYNARY_LENGTH);
			sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
		}

	}
		break;
	case PT_PLAYER_POSITION:
	{
		uint16_t id = 100;
		int16_t x, y;
		imbs.Read(&id, BINARYPACKETYPELENGTH);
		imbs.Read(&x, POSITION_BYNARY_LENGTH);
		imbs.Read(&y, POSITION_BYNARY_LENGTH);
		if (x != 0 && y != 0)
		{
			playerList[id]->getX() = x;
			playerList[id]->getY() = y;
		}
	}
		break;
	case PT_PING:
		
		
			for (int i = 0; i < playerList.size(); i++)
			{
				
					OutputMemoryBitStream ombs;

					ombs.Write(PacketType::PT_PLAYER_POSITION, BINARYPACKETYPELENGTH);
					ombs.Write(i, BINARYPACKETYPELENGTH);
					ombs.Write(playerList[i]->getX(), POSITION_BYNARY_LENGTH);
					ombs.Write(playerList[i]->getY(), POSITION_BYNARY_LENGTH);
					
					sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
					sf::sleep(sf::milliseconds(200));


			}
		
		break;
	case PT_DISCONNECT:
	{
		std::cout << "Solcitud de desconexion de Cliente: " << ip << "Con puerto: " << port << std::endl;
		OutputMemoryBitStream ombs;
		ombs.Write(PacketType::PT_DISCONNECT, BINARYPACKETYPELENGTH);
		sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
			int p = port;
			for (int i = 0; i < playerList.size(); i++)
			{
				if (playerList[i]->getPlayerPort() == port)
				{
					playerList.erase(playerList.begin() + i);
					break;
				}
			}


		}
	
	

	
		break;
	default:
		std::cout << "Tipo de paquete no identificado, por favor revisa lo que se envía." << std::endl;

	}

	
}





bool UDPmanager::sendMessage(char * message, sf::IpAddress & ip, unsigned short & port,uint32_t sizeBuffer)
{
		sf::sleep(sf::milliseconds(200));
		sf::Packet packet;
		packet.append(message, sizeBuffer);
		sf::Socket::Status status = socket.send(packet, ip, port);
		server_clock.restart(); //
		if (status == sf::Socket::Done)
			return true;
		else
			return false;
	
	
	

}

void UDPmanager::serverDisconnection()
{
	shutdown_clock.restart();
	std::cout << "Server Is About to ShutDown" << std::endl;
	while (playerList.size() != 0)
	{
		if (shutdown_clock.getElapsedTime() > sf::seconds(3.0))
		{
			for (int i = 0; i < playerList.size(); i++)
			{
				playerList.erase(playerList.begin() + i);

			}
			break;
		}
			for (int i = 0; i < playerList.size(); i++)
			{
			
					OutputMemoryBitStream ombs;
					ombs.Write(PacketType::PT_SHUTDOWN, BINARYPACKETYPELENGTH);
					sf::IpAddress ip = playerList[i]->getPlayerIP();
					unsigned short port = playerList[i]->getPlayerPort();
					sendMessage(ombs.GetBufferPtr(), ip, port, ombs.GetByteLength());
				
			}

			
		
	}
	isConnected = false;
}


