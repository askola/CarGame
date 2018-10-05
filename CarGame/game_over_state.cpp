#include "game_over_state.hpp"
#include "menu_state.hpp"
#include "state_machine.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream> // For debugging

GameOverState::GameOverState( StateMachine& machine, sf::RenderWindow& window, bool replace )
: State{ machine, window, replace }
{
    std::cout << "GameOverState Init" << std::endl;
    
    if(!m_bgTex.loadFromFile( "./img/score_screen.png" ))
        std::cerr << "Error loading score_screen.png" << std::endl;
    m_bg.setTexture( m_bgTex, true );

}

void GameOverState::pause()
{
    std::cout << "GameOverState Pause" << std::endl;
}

void GameOverState::resume()
{
    std::cout << "GameOverState Resume" << std::endl;
}

void GameOverState::update(const float dt)
{
    (void) dt;
    sf::Event event;

    while( m_window.pollEvent( event ) )
    {

        switch( event.type )
        {

            case sf::Event::Closed:
            {
                m_machine.quit();
                break;
            }

            // Add key functions here
            case sf::Event::KeyPressed:
            {
                switch( event.key.code )
                {

                    case sf::Keyboard::Space:
                    {
                        m_next = StateMachine::build<MenuState>( m_machine, m_window, true );
                        break;
                    }

                    default:
                        break;
                }
                break;
            }

            default:
                break;
        }
    }
}

void GameOverState::draw()
{
    // Clear previous drawing
    m_window.clear();
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    m_window.setView(view);
    m_window.draw( m_bg );
    m_window.display();
}
