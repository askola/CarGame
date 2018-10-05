#include "map.hpp"
#include <utility>
#include <iostream>
#include <fstream>

//Text files are located in /track file

std::vector<std::pair<float, float>> Map::RoadVector(std::string mapname){
	// This function loads the map points from .txt file
	std::ifstream inData;
	inData.open(mapname);
	
	if (inData.fail())
		std::cout << "Error opening map file" << std::endl;
		
	char ignoreline[256];
	int lines;
	inData >> lines;
	inData.getline(ignoreline, 256);
	
	//Points for track
	std::vector<std::pair<float, float>> track;
	
	float x,y;
	for (int i=0; i<lines; i++){
	inData >> x >> y; //Read x and y coordinates at position i
	std::pair<float,float> buffer(x,y);
	track.push_back(buffer);
}

	inData.close();
	
	return track;
}

std::vector<std::pair<float, float>> Map::SolidVector(std::string mapname){
	//This function loads the solid obstacle points from .txt file
	std::ifstream inData;
	inData.open(mapname);
	
	if (inData.fail())
		std::cout << "Error opening map file" << std::endl;
		
	char ignoreline[256];
	int lines, solid;
	inData >> lines;
	inData.getline(ignoreline, 256);
	
	//Points for track
	std::vector<std::pair<float, float>> vec;
	
	float x,y;
	for (int i=0; i<lines; i++)
	inData.getline(ignoreline, 256);

	inData >> solid;
	inData.getline(ignoreline, 256);
	for (int i=0; i<solid; i++){
	inData >> x >> y; //Read x and y coordinates at position i
	std::pair<float,float> buffer(x,y);
	vec.push_back(buffer);
}

	inData.close();
	
	return vec;
}

std::vector<std::pair<float, float>> Map::OilVector(std::string mapname){
	//This function loads the oil obstacle points from .txt file
	std::ifstream inData;
	inData.open(mapname);
	
	if (inData.fail())
		std::cout << "Error opening map file" << std::endl;
		
	char ignoreline[256];
	int lines, solid, oil;
	inData >> lines;
	inData.getline(ignoreline, 256);
	
	//Points for track
	std::vector<std::pair<float, float>> vec;
	
	float x,y;
	for (int i=0; i<lines; i++)
	inData.getline(ignoreline, 256);

	inData >> solid;
	inData.getline(ignoreline, 256);
	for (int i=0; i<solid; i++)
	inData.getline(ignoreline, 256);
	
	inData >> oil;
	inData.getline(ignoreline, 256);
	for (int i=0; i<oil; i++){
	inData >> x >> y; //Read x and y coordinates at position i
	std::pair<float,float> buffer(x,y);
	vec.push_back(buffer);
}

	inData.close();
	
	return vec;
}

std::vector<std::pair<float, float>> Map::CarVector(std::string mapname){
	//This function loads the car points from .txt file
	std::ifstream inData;
	inData.open(mapname);
	
	if (inData.fail())
		std::cout << "Error opening map file" << std::endl;
		
	char ignoreline[256];
	int lines, solid, oil, car;
	inData >> lines;
	inData.getline(ignoreline, 256);
	
	//Points for track
	std::vector<std::pair<float, float>> vec;
	
	float x,y;
	for (int i=0; i<lines; i++)
	inData.getline(ignoreline, 256);

	inData >> solid;
	inData.getline(ignoreline, 256);
	for (int i=0; i<solid; i++)
		inData.getline(ignoreline, 256);
	
	inData >> oil;
	inData.getline(ignoreline, 256);
	for (int i=0; i<oil; i++)
		inData.getline(ignoreline, 256);

	inData >> car;
	inData.getline(ignoreline, 256);
	for (int i=0; i<car; i++){
	inData >> x >> y; //Read x and y coordinates at position i
	std::pair<float,float> buffer(x*SCALE,y*SCALE); //Car coordinates need to be scaled
	vec.push_back(buffer);
}

	inData.close();
	return vec;
}

std::vector<std::pair<float, float>> Map::RoadVector2(std::string mapname){
	//Calculate outer points for the track from the first RoadVector
	std::vector<std::pair<float, float>> track = RoadVector(mapname);
	std::vector<std::pair<float, float>> track2;
	
	/*Move each point 15 meters away to create second side of the track,
	determines how thick the track is */
	float M = 15;
	
	//Here we use some math to calculate distances
 	for (unsigned int i = 0; i < track.size(); i++){	
	float x1 = track[i].first;
	float y1 = track[i].second;
	float x,y;
	if (x1<0)
		x = x1 - M;
	else
		x = x1 + M;
		
	if (y1<0)
		y = y1 - M;
	else
		y = y1 + M;
	std::pair <float, float> point(x,y);    
	track2.push_back(point);
	
	}
	
	return track2;
}

void Map::createWorld(b2World *world, std::string mapname){

	//center is the main body for the track
	b2Body* center;
	b2BodyDef trackbody;
	trackbody.position.Set(0,0);
    
	center = world->CreateBody(&trackbody);
	
	//Points for track
	std::vector<std::pair<float, float>> points = RoadVector(mapname);
	unsigned int size = points.size();
	std::vector<b2Vec2> track;

	for (unsigned int i = 0; i < size; i++){
	    b2Vec2 point(points[i].first,points[i].second);
	    track.push_back(point);
	}
	
	//Create edges from point-to-point
 	for (unsigned int i = 0; i < size - 1; i++){
	    b2FixtureDef fixt;	
	    b2PolygonShape edgeShape;
	    edgeShape.SetAsEdge( track[i], track[i+1] );
	    fixt.shape = &edgeShape;
	    center->CreateFixture(&fixt);
	}
	
	// Connect the last points in the track
    b2FixtureDef fixt;	
	b2PolygonShape edgeShape;
	edgeShape.SetAsEdge( track[size - 1], track[0] );
	fixt.shape = &edgeShape;
	center->CreateFixture(&fixt);
	

	//Same procedure as before, now we just use the outer points
	
	//Points for track
	std::vector<std::pair<float, float>> points2 = RoadVector2(mapname);
	unsigned int size2 = points2.size();

	std::vector<b2Vec2> track2;

	for (unsigned int i = 0; i < size2; i++){
  		
  		b2Vec2 point(points2[i].first,points2[i].second);
  		track2.push_back(point);
	}
	
	
	for (unsigned int i = 0; i < size2 -1; i++){
	b2PolygonShape edgeShape;
	edgeShape.SetAsEdge( track2[i], track2[i+1] );
	fixt.shape = &edgeShape;
	center->CreateFixture(&fixt);
	}	

	edgeShape.SetAsEdge( track2[size2 - 1], track2[0] );
	fixt.shape = &edgeShape;
	center->CreateFixture(&fixt);

	//Create 4 checkpoints to the world and give them their respective filter bits
	b2BodyDef check;
	b2PolygonShape cpShape;
	
	b2Body* checkpoint = world->CreateBody(&check);
	checkpoint->SetUserData(this);
	
	int point = size2/4;
	//Checkpoint 1
	b2FixtureDef cp1;
	cpShape.SetAsEdge( track[0], track2[0] );
	cp1.isSensor = true;
	cp1.shape = &cpShape;
	cp1.filter.categoryBits = FIRST;
	checkpoint->CreateFixture(&cp1);
	
	//Checkpoint 2
	b2FixtureDef cp2;	
	cpShape.SetAsEdge( track[point], track2[point] );
	cp2.isSensor = true;
	cp2.shape = &cpShape;
	cp2.filter.categoryBits = SECOND;
	checkpoint->CreateFixture(&cp2);
	
	//Checkpoint 4
	b2FixtureDef cp3;	
	cpShape.SetAsEdge( track[point*2], track2[point*2] );
	cp3.isSensor = true;
	cp3.shape = &cpShape;
	cp3.filter.categoryBits = THIRD;
	checkpoint->CreateFixture(&cp3);
	
	b2FixtureDef cp4;	
	cpShape.SetAsEdge( track[point*3], track2[point*3] );
	cp4.isSensor = true;
	cp4.shape = &cpShape;
	cp4.filter.categoryBits = FOURTH;
	checkpoint->CreateFixture(&cp4);
}

sf::ConvexShape Map::convin(unsigned int i, std::string mapname){
	//This creates the inner grass for the map in SFML
	
	//Points for track
	std::vector<std::pair<float, float>> points = RoadVector(mapname);
	unsigned int size = points.size();
	//Creating convex inside the track
	sf::ConvexShape convex;
	convex.setPointCount(3);
	
	if (i == points.size() - 1){
	convex.setPoint(0, sf::Vector2f(points[size - 1].first*SCALE, points[size - 1].second*SCALE));
	convex.setPoint(1, sf::Vector2f(points[0].first*SCALE, points[0].second*SCALE));
	convex.setPoint(2, sf::Vector2f(0,0));
}
	else{
	convex.setPoint(0, sf::Vector2f(points[i].first*SCALE, points[i].second*SCALE));
	convex.setPoint(1, sf::Vector2f(points[i+1].first*SCALE, points[i+1].second*SCALE));
	convex.setPoint(2, sf::Vector2f(0,0));
}

	return convex;
}

sf::ConvexShape Map::convroad(unsigned int i, std::string mapname){
	//This creates the road for the map in SFML
	
	std::vector<std::pair<float, float>> points = RoadVector2(mapname);
	unsigned int size = points.size();
	//Creating convex for the outer convex
	sf::ConvexShape convex;
	convex.setPointCount(3);
	
	if (i == (points.size() - 1)){
	convex.setPoint(0, sf::Vector2f(points[size - 1].first*SCALE, points[size - 1].second*SCALE));
	convex.setPoint(1, sf::Vector2f(points[0].first*SCALE, points[0].second*SCALE));
	convex.setPoint(2, sf::Vector2f(0,0));
}
	else{
	convex.setPoint(0, sf::Vector2f(points[i].first*SCALE, points[i].second*SCALE));
	convex.setPoint(1, sf::Vector2f(points[i+1].first*SCALE, points[i+1].second*SCALE));
	convex.setPoint(2, sf::Vector2f(0,0));
}
	
	return convex;
}

sf::ConvexShape Map::convout(unsigned int i, std::string mapname){
	/* This creates the outer grass for map in SFML by creating 16
	underlying boxes */
	
	sf::ConvexShape convex;
	convex.setPointCount(4);
	std::vector<std::pair<float, float>> points = RoadVector2(mapname);
	
	if (i == 0){
	convex.setPoint(0, sf::Vector2f(75*SCALE, 150*SCALE));
	convex.setPoint(1, sf::Vector2f(150*SCALE, 150*SCALE));
	convex.setPoint(2, sf::Vector2f(150*SCALE,75*SCALE));
	convex.setPoint(3, sf::Vector2f(75*SCALE, 75*SCALE));
}
	
	if (i == 1){
	convex.setPoint(0, sf::Vector2f(75*SCALE, 75*SCALE));
	convex.setPoint(1, sf::Vector2f(150*SCALE, 75*SCALE));
	convex.setPoint(2, sf::Vector2f(150*SCALE, 0*SCALE));
	convex.setPoint(3, sf::Vector2f(75*SCALE, 0*SCALE));
}
	
	if (i == 2){
	convex.setPoint(0, sf::Vector2f(75*SCALE, 0*SCALE));
	convex.setPoint(1, sf::Vector2f(150*SCALE, 0*SCALE));
	convex.setPoint(2, sf::Vector2f(150*SCALE,-75*SCALE));
	convex.setPoint(3, sf::Vector2f(75*SCALE, -75*SCALE));
}
	
	if (i == 3){
	convex.setPoint(0, sf::Vector2f(75*SCALE, -75*SCALE));
	convex.setPoint(1, sf::Vector2f(150*SCALE, -75*SCALE));
	convex.setPoint(2, sf::Vector2f(150*SCALE,-150*SCALE));
	convex.setPoint(3, sf::Vector2f(75*SCALE, -150*SCALE));
}
	if (i == 4){
	convex.setPoint(0, sf::Vector2f(75*SCALE, -75*SCALE));
	convex.setPoint(1, sf::Vector2f(75*SCALE, -150*SCALE));
	convex.setPoint(2, sf::Vector2f(0*SCALE,-150*SCALE));
	convex.setPoint(3, sf::Vector2f(0*SCALE, -75*SCALE));
}
	
	if (i == 5){
	convex.setPoint(0, sf::Vector2f(0*SCALE, 0*SCALE));
	convex.setPoint(1, sf::Vector2f(75*SCALE, 0*SCALE));
	convex.setPoint(2, sf::Vector2f(75*SCALE, -75*SCALE));
	convex.setPoint(3, sf::Vector2f(0*SCALE, -75*SCALE));
}
	
	if (i == 6){
	convex.setPoint(0, sf::Vector2f(0*SCALE, 0*SCALE));
	convex.setPoint(1, sf::Vector2f(75*SCALE, 0*SCALE));
	convex.setPoint(2, sf::Vector2f(75*SCALE, 75*SCALE));
	convex.setPoint(3, sf::Vector2f(0*SCALE, 75*SCALE));
}
	
	if (i == 7){
	convex.setPoint(0, sf::Vector2f(0*SCALE, 75*SCALE));
	convex.setPoint(1, sf::Vector2f(75*SCALE, 75*SCALE));
	convex.setPoint(2, sf::Vector2f(75*SCALE,150*SCALE));
	convex.setPoint(3, sf::Vector2f(0*SCALE, 150*SCALE));
}
	if (i == 8){
	convex.setPoint(0, sf::Vector2f(-75*SCALE, 150*SCALE));
	convex.setPoint(1, sf::Vector2f(-150*SCALE, 150*SCALE));
	convex.setPoint(2, sf::Vector2f(-150*SCALE,75*SCALE));
	convex.setPoint(3, sf::Vector2f(-75*SCALE, 75*SCALE));
}
	
	if (i == 9){
	convex.setPoint(0, sf::Vector2f(-75*SCALE, 75*SCALE));
	convex.setPoint(1, sf::Vector2f(-150*SCALE, 75*SCALE));
	convex.setPoint(2, sf::Vector2f(-150*SCALE, 0*SCALE));
	convex.setPoint(3, sf::Vector2f(-75*SCALE, 0*SCALE));
}
	
	if (i == 10){
	convex.setPoint(0, sf::Vector2f(-75*SCALE, 0*SCALE));
	convex.setPoint(1, sf::Vector2f(-150*SCALE, 0*SCALE));
	convex.setPoint(2, sf::Vector2f(-150*SCALE,-75*SCALE));
	convex.setPoint(3, sf::Vector2f(-75*SCALE, -75*SCALE));
}
	
	if (i == 11){
	convex.setPoint(0, sf::Vector2f(-75*SCALE, -75*SCALE));
	convex.setPoint(1, sf::Vector2f(-150*SCALE, -75*SCALE));
	convex.setPoint(2, sf::Vector2f(-150*SCALE,-150*SCALE));
	convex.setPoint(3, sf::Vector2f(-75*SCALE, -150*SCALE));
}
	if (i == 12){
	convex.setPoint(0, sf::Vector2f(-75*SCALE, -75*SCALE));
	convex.setPoint(1, sf::Vector2f(-75*SCALE, -150*SCALE));
	convex.setPoint(2, sf::Vector2f(-0*SCALE,-150*SCALE));
	convex.setPoint(3, sf::Vector2f(-0*SCALE, -75*SCALE));
}
	
	if (i == 13){
	convex.setPoint(0, sf::Vector2f(-0*SCALE, 0*SCALE));
	convex.setPoint(1, sf::Vector2f(-75*SCALE, 0*SCALE));
	convex.setPoint(2, sf::Vector2f(-75*SCALE, -75*SCALE));
	convex.setPoint(3, sf::Vector2f(-0*SCALE, -75*SCALE));
}
	
	if (i == 14){
	convex.setPoint(0, sf::Vector2f(-0*SCALE, 0*SCALE));
	convex.setPoint(1, sf::Vector2f(-75*SCALE, 0*SCALE));
	convex.setPoint(2, sf::Vector2f(-75*SCALE, 75*SCALE));
	convex.setPoint(3, sf::Vector2f(-0*SCALE, 75*SCALE));
}
	
	if (i == 15){
	convex.setPoint(0, sf::Vector2f(-0*SCALE, 75*SCALE));
	convex.setPoint(1, sf::Vector2f(-75*SCALE, 75*SCALE));
	convex.setPoint(2, sf::Vector2f(-75*SCALE,150*SCALE));
	convex.setPoint(3, sf::Vector2f(-0*SCALE, 150*SCALE));
}
	return convex;
	
}

sf::RectangleShape Map::finishline(std::string mapname){
	/*Create the finish line, we divide the first and second point distance in half
	until we get desired distance */
	
	std::vector<std::pair<float, float>> points1 = RoadVector(mapname);
	std::vector<std::pair<float, float>> points2 = RoadVector2(mapname);
	
	std::pair<float, float> p1 = points1[0];
	std::pair<float, float> p2 = points2[0];

	float distance = sqrt((p1.first - p2.first)*(p1.first - p2.first)+(p1.second - p2.second)*(p1.second - p2.second));
	
	float angle = atan((p1.second - p2.second)/(p1.first - p2.first)) * RADTODEG;
	
	std::cout << "Angle is : "<<angle << std::endl;
	
	sf::RectangleShape rectangle(sf::Vector2f(SCALE*distance, SCALE*5.f));
	

	rectangle.setPosition(p1.first*SCALE, p1.second*SCALE);
	rectangle.setRotation(angle);
	
	return rectangle;
}
