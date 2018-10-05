#include "obstacles.hpp"

sf::CircleShape Obstacles::solid(float x, float y, float radius, b2World *m_world){
	//Create solid obstacle that car can not pass
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
    b2Body* m_groundBody = m_world->CreateBody( &bodyDef );

    b2CircleShape circleShape;
    circleShape.m_radius = radius;
             
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.friction = 1;
    
    m_groundBody->CreateFixture(&fixtureDef);

	sf::CircleShape circle;
	
	circle.setRadius(radius*SCALE);
	circle.setOrigin(radius*SCALE,radius*SCALE);
	circle.setPosition(sf::Vector2f(x*SCALE, y*SCALE));
	
	return circle;
}

sf::CircleShape Obstacles::oil(float x, float y, float radius, b2World *m_world){
	//Create oil obstacle
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(x, y);
    b2Body* m_groundBody = m_world->CreateBody( &bodyDef );

    b2CircleShape circleShape;
    circleShape.m_radius = radius;
             
    b2FixtureDef fixtureDef;
    fixtureDef.isSensor = true;
    fixtureDef.shape = &circleShape;
	fixtureDef.filter.categoryBits = OIL;
    fixtureDef.friction = 1;
    
    m_groundBody->CreateFixture(&fixtureDef);

	sf::CircleShape circle;
	
	circle.setRadius(radius*SCALE);
	circle.setOrigin(radius*SCALE,radius*SCALE);
	circle.setPosition(sf::Vector2f(x*SCALE, y*SCALE));
	circle.setFillColor(sf::Color::Black);
	return circle;
}
