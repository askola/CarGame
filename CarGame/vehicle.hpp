#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>


#include <iostream> // For debugging
#include "Box2D/Box2D.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

class Vehicle
{
public:


    Vehicle(b2World* world, unsigned int i = 0, float x = 0.0, float y = 0.0,  float mass = 100, float force = 20, float backForce = 5, float maxV = 40, float maxBV = -20, float friction = 1.0);
    void throttle();
    void brake();
    void turnLeft();
    void turnRight();
    void drag();
    float getCurrentSpeed();
    b2Vec2 getLateralVelocity();
    b2Vec2 getForwardVelocity();
    sf::Sprite& getSprite();
    sf::Text& getText();
    b2Body* getBody();
    void updateFriction();
    bool angularv;
    void hitOil();
    int checkpoints();
    void updateEffectTime(const float dt);
    void looseOil();
    bool getOil();
    
    //Timer functions
    void setLap();
    int getLap();
    sf::Clock& getLapClock();
    std::string getLapFormat();
    
    int getLastCheck();
    void setLastCheck(int);
    void increaseLap();
    
private:

    sf::Texture m_carTex;
    sf::Sprite m_car;
    sf::Text m_text; //for mini map
    sf::Font m_font; //for mini map
    b2Body* m_myBody;
    float m_maxVelocity;
    float m_maxBackVelocity;
    float m_friction;
    float m_force;
    float m_backForce;
    float m_maxLateralImpulse;
    float m_maxAngularImpulse;
    bool m_boost;
    bool m_oil;
    float m_effectTime;
    
    //Laps and timer
    std::vector<std::string> m_laps;
    std::string m_lapFormat, m_lapMins, m_lapSecs, m_lapMilSecs;
    sf::Clock m_lapClock;
    int m_lap = 0;
    int m_lastCheck = 4;
    
    
};

#endif /* VEHICLE_HPP */

