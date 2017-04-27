#pragma once
#include "SFML/System.hpp"


#define DIMENSION_X 5
#define DIMENSION_Y 5
class Grid
{
private:
	int grid[DIMENSION_X][DIMENSION_Y];
public:
	void setCharacter(int x, int y);
	
	void setCharacter(sf::Vector2<int> v);

	void removeCharacter(int x, int y);

	void removeCharacter(sf::Vector2<int> v);
	
	Grid();
	~Grid();
};

