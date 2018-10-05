#include "game.hpp"

/*
    Every function is a member function ->
    When leaving the scope of Game the engine will be cleaned up!
    
*/

Game::Game()
{
    m_window.setFramerateLimit( 60 );
}

// Setup initial game settings
void Game::run()
{
    // Create render window
    m_window.create( sf::VideoMode{800, 600}, "Engine Test Window", sf::Style::Titlebar | sf::Style::Close );
    
    // Set mouse cursor invisible
    m_window.setMouseCursorVisible(false);
    
    // Game Engine init
    m_machine.run( StateMachine::build<MenuState>( m_machine, m_window, true ) );
    
    // Main gameLoop
    
    sf::Clock clock;
    
    while ( m_machine.running() )
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        
        m_machine.nextState();
        m_machine.update( dt );
        m_machine.draw();
    }
}

