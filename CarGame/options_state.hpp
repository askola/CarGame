#ifndef OPTIONS_STATE_HPP
#define OPTIONS_STATE_HPP

#include "state.hpp"

// SFML specific headers
#include <SFML/Graphics.hpp>

#define m_OPTIONS_ITEMS 3 // FIXME

class StateMachine;

namespace sf { class RenderWindow; }

class OptionsState : public State
{
public:

    OptionsState( StateMachine& machine, sf::RenderWindow& window, bool replace = true );

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
    sf::Text m_title;
    sf::Font m_font;
    sf::Font m_font_shadow;
    sf::Text m_menu[m_OPTIONS_ITEMS];
    int m_selectedItemIndex;

};

#endif /* OPTIONS_STATE_HPP */
