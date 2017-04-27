#include "SFML\System.hpp"
#include "SFML\Graphics.hpp"
#define RADIUS 60
#define GRAVITY 2
#define jumpHeight 10.0f
class character 
{
private:
	sf::Vector2<int16_t> position;
	sf::Texture circle_texture;
	sf::CircleShape* circle_shape;
	sf::Vector2<int16_t> velocity;
	sf::Clock jump_clock;
	bool onGround;
	//character();
public:

	character(int16_t x, int16_t y);
	character(sf::Vector2<int16_t> pos);
	~character();

	sf::Vector2<int16_t> & getPosition();
	void setPosition(sf::Vector2<int16_t> & v);
	sf::CircleShape* getCircleshape();
	void updatePosition();
	sf::Vector2<int16_t> & getVelocity();
	void jump();
	void restartTime();

	sf::Clock & getClock();
};