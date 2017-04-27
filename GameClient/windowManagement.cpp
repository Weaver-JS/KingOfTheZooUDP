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
	

	

}

void windowManagement::loop()
{
	
	sf::Thread recv_thread(&UDPmanager::recv, &udpManager);
	if (!udpManager.isDisconnected())
	{

		int16_t xU = 0;
		int16_t yU = 0;
		main_character->setPosition(sf::Vector2<int16_t>(xU, yU));
		playerList.push_back(main_character);
	
		//playerList[udpManager.getPlayer().getPlayerID()]->restartTime();
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
			
//PING
			if(timer_client.getElapsedTime() > sf::milliseconds(200))
			{
				
				

				//NETWORKING
				if (playerList.size() < udpManager.getPlayerSize())
				{
					for (int i = playerList.size(); i < udpManager.getPlayerSize(); i++)
					{
						character* p = new character(0, 0);
						playerList.push_back(p);
					
					}
				}
				udpManager.ping(playerList[udpManager.getPlayer().getPlayerID()]->getPosition().x,
					playerList[udpManager.getPlayer().getPlayerID()]->getPosition().y);

				for (int i = 0; i < playerList.size(); i++)
				{
					int16_t x = udpManager.getPlayer(i).getX(), y = udpManager.getPlayer(i).getY();
					playerList[i]->setPosition(sf::Vector2<int16_t>(x, y));
					
				}
				timer_client.restart();
			}
			//PING
		/*	for (int i = 0; i < accumlist.getMovementList().size(); i++)
			{
				udpManager.sendAccumList(accumlist.getMovementList());
			}*/
/*
			if (playerList.size() < udpManager.getPlayerSize())
			{
				for (int i = playerList.size(); i < udpManager.getPlayerSize(); i++)
				{
					character* p = new character(0, 0);
					playerList.push_back(p);
					//playerList.resize(udpManager.getPlayerSize());
				}
			}
			for (int i = 0; i < playerList.size(); i++)
			{
				int16_t x = udpManager.getPlayer(i).getX(), y = udpManager.getPlayer(i).getY();
				playerList[i]->setPosition(sf::Vector2<int16_t>(x, y));

			}
			*/
			for (int i = 0; i < playerList.size(); i++)
			{
				
				//playerList[i]->updatePosition();
				windowRenderer.draw(*playerList[i]->getCircleshape());
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
			accumlist.addMovement(movKey::RIGHT);
			
		}
		if (evento.key.code == sf::Keyboard::D)
		{
			accumlist.addMovement(movKey::LEFT);
			
		}

	case sf::Event::MouseButtonPressed:

		playerList[udpManager.getPlayer().getPlayerID()]->setPosition((sf::Vector2<int16_t>((sf::Mouse::getPosition(windowRenderer).x - main_character->getCircleshape()->getRadius()),
			 (sf::Mouse::getPosition(windowRenderer).y - main_character->getCircleshape()->getRadius()))));
			
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
