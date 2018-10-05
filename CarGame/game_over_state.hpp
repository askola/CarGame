#ifndef GAME_OVER_STATE_HPP
#define GAME_OVER_STATE_HPP

#include "state.hpp"

// SFML specific headers
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class StateMachine;

namespace sf { class RenderWindow; }

class GameOverState : public State
{
public:
    GameOverState( StateMachine& machine, sf::RenderWindow& window, bool replace = true );

    void pause();
    void resume();

    void update( const float dt );
    void draw();

private:

    sf::Texture m_bgTex;
    sf::Sprite m_bg;

};

#endif /* GAME_OVER_STATE_HPP */
