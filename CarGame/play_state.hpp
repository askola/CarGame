#ifndef PLAY_STATE_HPP
#define PLAY_STATE_HPP

#include "state.hpp"
#include "vehicle.hpp"
#include "track/map.hpp"

#include <list>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

class StateMachine;

namespace sf { class RenderWindow; }

class PlayState : public State
{
public:

    PlayState( StateMachine& machine, sf::RenderWindow& window, bool replace = true );

    void pause();
    void resume();

    void update( const float dt );
    void draw();

private:

    // SFML
    sf::Texture m_bgTex;
    sf::Sprite m_bg;
    sf::Sprite m_rectangle;
    sf::RectangleShape m_finishline;

	sf::Texture m_roadTexture;
	sf::Texture m_grassTexture;
	sf::Texture m_finishlineTexture;
	sf::Texture m_obstacleTexture;

    // Box2D
    b2World* m_world;
    Map* m_map = new Map;
    
    // Cars
    std::vector<Vehicle*> Cars;
    
    // Convexshapes
    std::vector<sf::CircleShape> m_objects;
    std::vector<sf::ConvexShape> m_convexShapes;

    // Start
    sf::Font m_font;
    sf::Text m_start_text;
    sf::Text m_timer;
    sf::Text m_lapTimer;
    sf::Clock m_clock;
    sf::Time m_raceTime;

    // Timer related params
    std::string m_raceFormat, m_raceMins, m_raceSecs;

    bool m_raceCntDwn = true;
    bool m_raceInit = true;
    bool m_firstLap = true;
    int m_counter = 0;

};

#endif /* PLAY_STATE_HPP */
