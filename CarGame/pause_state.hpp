#ifndef PAUSE_STATE_HPP
#define PAUSE_STATE_HPP

#include "state.hpp"

// SFML specific headers
#include <SFML/Graphics.hpp>

#define m_PAUSE_ITEMS 3

class StateMachine;

namespace sf { class RenderWindow; }

class PauseState : public State
{
public:
    PauseState( StateMachine& machine, sf::RenderWindow& window, bool replace = true );

    // Local menu traversing
    void moveUp();
    void moveDown();
    int getSelectedItem() { return m_selectedItemIndex; };
    
    // Inherited functions
    void pause();
    void resume();

    void update( const float dt );
    void draw();

private:

    sf::Texture m_bgTex;
    sf::Sprite m_bg;
    
    // Menu items
    sf::Font m_font;
    sf::Font m_font_shadow;
    
    sf::Text m_menu[m_PAUSE_ITEMS];
    sf::Text m_title;
    
    int m_selectedItemIndex;
    

};

#endif /* PAUSE_STATE_HPP */
