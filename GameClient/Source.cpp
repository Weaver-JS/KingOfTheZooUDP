#pragma once
#include <PlayerInfo.h>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <iostream>
#include "windowManagement.h"

int main()
{
	windowManagement::get().init(800, 800, "chat");
	windowManagement::get().loop();

	
	
	
	
	return 0;
}