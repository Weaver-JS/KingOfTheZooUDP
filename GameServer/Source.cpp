#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include "UDPmanager.h"





int main()
{
	UDPmanager udp;
	sf::Thread command_thread(&UDPmanager::throw_command,&udp);
	command_thread.launch();
	udp.initConnection();
	command_thread.terminate();
	return 0;
}

