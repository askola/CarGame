#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "state.hpp"

// SFML specific headers
#include <SFML/Graphics.hpp>

#define m_MENU_ITEMS 3

class StateMachine;

namespace sf { class RenderWindow; }

class MenuState : public State
{
public:

    MenuState( StateMachine& machine, sf::RenderWindow& window, bool replace = true );

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
    sf::Text m_menu[m_MENU_ITEMS];
    int m_selectedItemIndex;
    

};

#endif /* MENU_STATE_HPP */
