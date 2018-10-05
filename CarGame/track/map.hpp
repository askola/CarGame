#ifndef MAP_HPP
#define MAP_HPP

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#define SCALE 30.f
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

//Bits for checkpoints
enum _checkpointCategory {
    FIRST =     0x0001,
    SECOND =    0x0002,
    THIRD =     0x0004,
    FOURTH = 	0x0008,
};

class Map
{
	public:
	Map() {}
	~Map() {}
	std::vector<std::pair<float, float>> RoadVector(std::string mapname);
	std::vector<std::pair<float, float>> RoadVector2(std::string mapname);
	std::vector<std::pair<float, float>> OilVector(std::string mapname);
	std::vector<std::pair<float, float>> SolidVector(std::string mapname);
	std::vector<std::pair<float, float>> CarVector(std::string mapname);
	void createWorld(b2World *world, std::string mapname); // Creating Box2D World
	//int checkpoints(Vehicle *car);
	sf::RectangleShape finishline(std::string mapname);
	sf::ConvexShape convroad(unsigned int i,std::string mapname);
	sf::ConvexShape convin(unsigned int i,std::string mapname);
	sf::ConvexShape convout(unsigned int i,std::string mapname);

};

#endif
