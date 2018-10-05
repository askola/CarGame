#ifndef GAME_HPP
#define GAME_HPP

#include "menu_state.hpp"
#include "state_machine.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Game
{
public:

    Game();

    void run();

private:
    StateMachine m_machine;
    sf::RenderWindow m_window;
};
#endif /* GAME_HPP */

