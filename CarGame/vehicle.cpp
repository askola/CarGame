#include "vehicle.hpp"
#include "play_state.hpp"
#include "track/obstacles.hpp"


#include <cmath>

Vehicle::Vehicle(b2World* world, unsigned int i, float x, float y,  float mass, float force, float backForce, float maxV, float maxBV, float friction)
{
    std::cout << "Car init" << std::endl;
    
    m_maxVelocity = maxV;
    m_maxBackVelocity = maxBV;
    m_friction = friction;
    m_force = force;
    m_backForce = backForce;
    m_maxLateralImpulse = 20;
    m_maxAngularImpulse = 3;
    angularv = false;
    
    b2BodyDef myBodyDef;
    myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
    myBodyDef.position.Set(x/SCALE, y/SCALE); //set the starting position
    myBodyDef.angle = 180 * DEGTORAD; //set the starting angle
    
    m_myBody = world->CreateBody(&myBodyDef);
    
    m_myBody->SetUserData( this );
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox((34.f/2)/SCALE,(80.f/2)/SCALE);
    
    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &boxShape;
    boxFixtureDef.density = mass/10.f;
    
    m_myBody->CreateFixture(&boxFixtureDef);
    

    
    if(i == 0){
        
        if(!m_carTex.loadFromFile( "./img/BlueRaceCar.png" ))
            std::cerr << "Error loading BlueRaceCar.png" << std::endl;    
        
        if(!m_font.loadFromFile("fonts/justice.ttf")){
        
            std::cerr << "Error loading justice.ttf" << std::endl;
        }

        m_text.setFont(m_font);
        m_text.setString("P1");
        m_text.setCharacterSize(500);
        m_text.setColor(sf::Color::Blue);
        m_text.setStyle(sf::Text::Bold);

    }
    
    else if(i == 1){
        
        if(!m_carTex.loadFromFile( "./img/RedRaceCar.png" ))
            std::cerr << "Error loading RedRaceCar.png" << std::endl;
        
        if(!m_font.loadFromFile("fonts/justice.ttf")){
        
            std::cerr << "Error loading justice.ttf" << std::endl;
        }
        
        m_text.setFont(m_font);
        m_text.setString("P2");
        m_text.setCharacterSize(500);
        m_text.setColor(sf::Color::Red);
        m_text.setStyle(sf::Text::Bold);
    

    }
    
    else if(i == 2){
        
        if(!m_carTex.loadFromFile( "./img/YellowRaceCar.png" ))
            std::cerr << "Error loading YellowRaceCar.png" << std::endl;
        
        if(!m_font.loadFromFile("fonts/justice.ttf")){
        
            std::cerr << "Error loading justice.ttf" << std::endl;
        }
        
        m_text.setFont(m_font);
        m_text.setString("P3");
        m_text.setCharacterSize(500);
        m_text.setColor(sf::Color::Yellow);
        m_text.setStyle(sf::Text::Bold); 

    }
    
    else if(i == 3){
        
        if(!m_carTex.loadFromFile( "./img/GreenRaceCar.png" ))
            std::cerr << "Error loading GreenRaceCar.png" << std::endl;
            
        if(!m_font.loadFromFile("fonts/justice.ttf")){
        
            std::cerr << "Error loading justice.ttf" << std::endl;
        }
        
        
        m_text.setFont(m_font);
        m_text.setString("P4");
        m_text.setCharacterSize(500);
        m_text.setColor(sf::Color::Green);
        m_text.setStyle(sf::Text::Bold);
          

    }
    m_car.setTexture( m_carTex, true );
    m_car.setScale(0.1f,0.1f);

}

float Vehicle::getCurrentSpeed(){

    b2Vec2 currentForwardNormal = m_myBody->GetWorldVector( b2Vec2(0,1) );
    float currentSpeed = b2Dot( getForwardVelocity(), currentForwardNormal );
    return currentSpeed;

}

b2Vec2 Vehicle::getLateralVelocity() {

    b2Vec2 currentRightNormal = m_myBody->GetWorldVector( b2Vec2(1,0) );
    return b2Dot( currentRightNormal, m_myBody->GetLinearVelocity() ) * currentRightNormal;
}

b2Vec2 Vehicle::getForwardVelocity() {

    b2Vec2 currentForwardNormal = m_myBody->GetWorldVector( b2Vec2(0,1) );
    return b2Dot( currentForwardNormal, m_myBody->GetLinearVelocity() ) * currentForwardNormal;

}

void Vehicle::throttle(){
    b2Vec2 currentForwardNormal = m_myBody->GetWorldVector( b2Vec2(0,1) );
    float currentSpeed = getCurrentSpeed();
    float force = 0;
    
    if(currentSpeed < m_maxVelocity){
                                
        force = m_force;
        
        if(m_oil)
            force /= 2.f;           
                            
    }
    
    m_myBody->ApplyForce( (force * m_friction * m_myBody->GetMass() * currentForwardNormal), m_myBody->GetWorldCenter() );
}

void Vehicle::brake()
{
    b2Vec2 currentForwardNormal = m_myBody->GetWorldVector( b2Vec2(0,1) );
    float currentSpeed = getCurrentSpeed();
    float force = 0;
    
    if(currentSpeed > m_maxBackVelocity) {
    
        force = -m_backForce;   
        
        if(m_oil)
            force /= 2.f;           
                
    }
    
    m_myBody->ApplyForce(force * m_friction * m_myBody->GetMass() * currentForwardNormal, m_myBody->GetWorldCenter() );

}
void Vehicle::turnLeft(){

    float currentSpeed = getCurrentSpeed();
    float torque = 0;
    
    if(m_myBody->GetAngularVelocity() > -3){
    
         if(std::abs(currentSpeed) > 0.5f) torque = -450;
         if(std::abs(currentSpeed) > 10.f) torque = -250;
    }
    m_myBody->ApplyTorque(torque*m_friction);
    

}
void Vehicle::turnRight(){

    float currentSpeed = getCurrentSpeed();
    float torque = 0;
    
    if(m_myBody->GetAngularVelocity() < 3){
    
         if(std::abs(currentSpeed) > 0.5f) torque = 450;
         if(std::abs(currentSpeed) > 10.f) torque = 250;
         
    }
    m_myBody->ApplyTorque(torque*m_friction);
}

void Vehicle::drag(){

    b2Vec2 currentForwardNormal = m_myBody->GetWorldVector( b2Vec2(0,1) );
    float currentSpeed = getCurrentSpeed();
    float dragForceMagnitude = -10 * currentSpeed;
    
    if(m_oil)
        dragForceMagnitude /= 2.f; 
    
    m_myBody->ApplyForce( dragForceMagnitude * currentForwardNormal, m_myBody->GetWorldCenter() );


}

sf::Sprite& Vehicle::getSprite(){

    return m_car;

}

sf::Text& Vehicle::getText(){

    return m_text;
    
}

b2Body* Vehicle::getBody(){

    return m_myBody;

}

void Vehicle::updateFriction() {

    b2Vec2 impulse = m_myBody->GetMass() * -getLateralVelocity();
    float angularImpulse = -m_myBody->GetInertia()*m_myBody->GetAngularVelocity();
    if(impulse.Length() > m_maxLateralImpulse){
    
        impulse *= m_maxLateralImpulse / impulse.Length();
        if(m_oil)
            impulse.x /= 2.f; 
            impulse.y /= 2.f;
    }
    
    if(std::abs(angularImpulse) > m_maxAngularImpulse){
    
        angularImpulse *= m_maxAngularImpulse/std::abs(angularImpulse);
        if(m_oil)
            angularImpulse /= 2.f;
    }
    
    m_myBody->ApplyLinearImpulse( impulse, m_myBody->GetWorldCenter());
    m_myBody->ApplyAngularImpulse(angularImpulse);
}

void Vehicle::hitOil(){
    
    for (b2ContactEdge* edge = m_myBody->GetContactList(); edge; edge = edge->next){
	
	    if (edge->contact->GetFixtureB()->GetFilterData().categoryBits == OIL && edge->contact->IsTouching() && edge->contact->GetFixtureB()->IsSensor()){
	    
	    
	        m_oil = true;
	        m_effectTime = 0.0f;
	    
	    
	    }
		    
    }
}


int Vehicle::checkpoints(){
	/* Here we look for the checkpoints in the world, each checkpoint is
	identified by their categoryBit */
	for (b2ContactEdge* edge = m_myBody->GetContactList(); edge; edge = edge->next){
	
	if (edge->contact->GetFixtureA()->GetFilterData().categoryBits == FIRST && edge->contact->IsTouching() && edge->contact->GetFixtureA()->IsSensor())
		return 1;
	
	if (edge->contact->GetFixtureA()->GetFilterData().categoryBits == FOURTH && edge->contact->IsTouching() && edge->contact->GetFixtureA()->IsSensor())
		return 2;
	
	if (edge->contact->GetFixtureA()->GetFilterData().categoryBits == THIRD && edge->contact->IsTouching() && edge->contact->GetFixtureA()->IsSensor())
		return 3;

	
	if (edge->contact->GetFixtureA()->GetFilterData().categoryBits == SECOND && edge->contact->IsTouching() && edge->contact->GetFixtureA()->IsSensor())
		return 4;
}
	return 0;
}


void Vehicle::updateEffectTime(const float dt){


    m_effectTime += dt;


}

void Vehicle::looseOil(){

    if(m_effectTime > 5.0f){
    
        m_oil = false;
    
    }
    

}

bool Vehicle::getOil(){

    return m_oil;

}

void Vehicle::setLap(){

    m_laps.push_back(getLapFormat());

}

int Vehicle::getLap(){

    return m_lap;

}

sf::Clock& Vehicle::getLapClock(){

    return m_lapClock;

}

std::string Vehicle::getLapFormat(){

    // Move to Cars
    sf::Time m_lapTime = m_lapClock.getElapsedTime();
    
    m_lapMins = std::to_string((int) m_lapTime.asSeconds() % 60);
    m_lapSecs = std::to_string(m_lapTime.asMilliseconds() % 1000);
    m_lapMilSecs = std::to_string((m_lapTime.asMicroseconds() % 10) / 10);
    m_lapFormat = std::string("Lap ") + std::to_string((int) m_laps.size()) + " : " + m_lapMins + ":" + m_lapSecs + "'" + m_lapMilSecs;

    return m_lapFormat;
    
}

int Vehicle::getLastCheck(){

    return m_lastCheck;

}

void Vehicle::setLastCheck(int n){

    m_lastCheck = n;
    

}

void Vehicle::increaseLap(){

    m_lap += 1;

}

