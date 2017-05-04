#include "windowManagement.h"





void windowManagement::init(float x, float y, std::string chatName)
{
	
	timer_client.restart();
	screenDimensions.x = x;
	screenDimensions.y = y;
	

	main_character = new character(x/2,y/2);
	if (!font.loadFromFile("courbd.ttf"))
	{
		std::cout << "Can't load the font file" << std::endl;
	}
	
	
	udpManager.initConnection();
	
	windowRenderer.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), chatName);
	
//	playerList.resize(MAXPLAYERS);
	for (int i = 0; i < MAXPLAYERS; i++)
	{
		character p(0, 0);
		playerList.push_back(p);
		

	}


	

}

void windowManagement::loop()
{
	
	sf::Thread recv_thread(&UDPmanager::recv, &udpManager);



	if (!udpManager.isDisconnected())
	{

	
	
	
		recv_thread.launch();
		
		while (windowRenderer.isOpen())
		{
			
			sf::Event evento;
			while (windowRenderer.pollEvent(evento))
			{
				windowStuff(evento);
			}
			if (udpManager.isDisconnected())
			{
				windowRenderer.close();
			}
			windowRenderer.draw(separator);

			for (int i = 0; i < playerList.size(); i++)
			{
				int16_t x = udpManager.getPlayer(i).getX(), y = udpManager.getPlayer(i).getY();
				playerList[i].setPosition(sf::Vector2<int16_t>(x, y));

			}
//PING
			if(timer_client.getElapsedTime() > sf::milliseconds(200))
			{
				
				//NETWORKING
			
			//	playerList[udpManager.getPlayer().getPlayerID()].getPosition().x += accumlist.getFinalPosition();

					udpManager.ping(accumlist.getFinalPosition(),
					playerList[udpManager.getPlayer().getPlayerID()].getPosition().y,accumlist.getMovementCheckList().size());

				
				timer_client.restart();
			}
//PING

			
			for (int i = 0; i < playerList.size(); i++)
			{
				
				//playerList[i]->updatePosition();
				windowRenderer.draw(playerList[i].getCircleshape());
			}

				
		
			
			windowRenderer.display();
			windowRenderer.clear();
			
		}
	
		
	}
	recv_thread.terminate();
}

void windowManagement::windowStuff(sf::Event & evento)
{
	switch (evento.type)
	{
	case sf::Event::KeyPressed:
		if (evento.key.code == sf::Keyboard::Escape)
		{
			udpManager.disconnect();

		}

		if (evento.key.code == sf::Keyboard::A)
		{
			accumlist.addMovement(movKey::LEFT);
			
		}
		if (evento.key.code == sf::Keyboard::D)
		{
			accumlist.addMovement(movKey::RIGHT);
			
		}

	case sf::Event::MouseButtonPressed:

	/*	playerList[udpManager.getPlayer().getPlayerID()].setPosition((sf::Vector2<int16_t>((sf::Mouse::getPosition(windowRenderer).x - main_character->getCircleshape()->getRadius()),
			 (sf::Mouse::getPosition(windowRenderer).y - main_character->getCircleshape()->getRadius()))));*/
			
		break;

		
	}
}

sf::RenderWindow & windowManagement::getWindowRenderer()
{
	
	return get().windowRenderer;
}


windowManagement::~windowManagement()
{
}
