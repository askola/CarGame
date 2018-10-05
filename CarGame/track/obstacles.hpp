#ifndef OBSTACLES_HPP
#define OBSTACLES_HPP

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "map.hpp"

//Flag to find oil spots
enum _oilCategory {
    OIL =  0x0016,
};
  

class Obstacles{
public:
	Obstacles() {}
	~Obstacles() {}
	sf::CircleShape solid(float x, float y, float radius, b2World *world);
	sf::CircleShape oil(float x, float y, float radius, b2World *m_world);
	void oil(float x, float y, float size);
	
private:
    
    sf::Texture m_obstacleTex;
	
};

#endif
