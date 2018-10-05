#include "pause_state.hpp"
#include "options_state.hpp"
#include "menu_state.hpp"
#include "state_machine.hpp"

#include <iostream> // For debugging
#include <memory>

PauseState::PauseState( StateMachine& machine, sf::RenderWindow& window, bool replace )
: State{ machine, window, replace }
{
    std::cout << "PauseState Init" << std::endl;
    
    if(!m_bgTex.loadFromFile( "./img/menu.png" ))
        std::cerr << "Error loading menu.png" << std::endl;
    m_bg.setTexture( m_bgTex, true );
    
    // Set font
    if(!m_font.loadFromFile("./fonts/justice.ttf"))
        std::cerr << "Error loading font justice.ttf" << std::endl;
    if(!m_font_shadow.loadFromFile("./fonts/justice3d.ttf"))
        std::cerr << "Error loading font justice3d.ttf" << std::endl;

    // Set title
    m_title.setFont(m_font);
    m_title.setCharacterSize(80);
    m_title.setString("Game Paused");
    m_title.setColor( sf::Color::Black );
    // Center title
    sf::FloatRect m_titleRect = m_title.getLocalBounds();
    m_title.setOrigin(m_titleRect.left + m_titleRect.width/2.0f,
               m_titleRect.top  + m_titleRect.height/2.0f);
    m_title.setPosition(sf::Vector2f( (window.getSize().x/2.0f) , 80.f ));
    
    // Set Menu items
    m_menu[0].setFont(m_font_shadow);
    m_menu[0].setCharacterSize(50);
    m_menu[0].setString("Resume");
    m_menu[0].setColor( sf::Color::Black );
    m_menu[0].setPosition(sf::Vector2f( 60.f, (window.getSize().y - ( 120.f + ( 80.f ) * m_PAUSE_ITEMS)) ));
    
    m_menu[1].setFont(m_font);
    m_menu[1].setCharacterSize(50);
    m_menu[1].setString("Controls");
    m_menu[1].setColor( sf::Color::White );
    m_menu[1].setPosition(sf::Vector2f( 60.f, ( window.getSize().y - (120.f+(80.f)*(m_PAUSE_ITEMS-1))) ));
    
    m_menu[2].setFont(m_font);
    m_menu[2].setCharacterSize(50);
    m_menu[2].setString("Exit to Main Menu");
    m_menu[2].setColor( sf::Color::White );

    // Center the exit button
    sf::FloatRect m_menuRect = m_menu[2].getLocalBounds();
    m_menu[2].setOrigin(m_menuRect.left + m_menuRect.width/2.0f,
               m_menuRect.top  + m_menuRect.height/2.0f);
    m_menu[2].setPosition(sf::Vector2f( (window.getSize().x/2.0f) , (window.getSize().y - 80.f) ));
    
    // Init highlight
    m_selectedItemIndex = 0;
}

void PauseState::moveUp()
{
    if ( m_selectedItemIndex-1 >= 0 )
    {
        m_menu[m_selectedItemIndex].setColor( sf::Color::White );
        m_menu[m_selectedItemIndex].setFont(m_font);
        
        m_selectedItemIndex--;
        
        m_menu[m_selectedItemIndex].setColor( sf::Color::Black );
        m_menu[m_selectedItemIndex].setFont(m_font_shadow);
    }
}

void PauseState::moveDown()
{
    if ( m_selectedItemIndex+1 < m_PAUSE_ITEMS )
    {
        m_menu[m_selectedItemIndex].setColor( sf::Color::White );
        m_menu[m_selectedItemIndex].setFont(m_font);
        
        m_selectedItemIndex++;
        
        m_menu[m_selectedItemIndex].setColor( sf::Color::Black );
        m_menu[m_selectedItemIndex].setFont(m_font_shadow);
    }
}

void PauseState::pause()
{
    std::cout << "PauseState Pause" << std::endl;
}

void PauseState::resume()
{
    std::cout << "PauseState Resume" << std::endl;
}

void PauseState::update(const float dt)
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
            case sf::Event::KeyReleased:
            {
                switch( event.key.code )
                {
                    case sf::Keyboard::Up:
                    {
                        this->moveUp();
                        break;
                    }
                    
                    case sf::Keyboard::Down:
                    {
                        this->moveDown();
                        break;
                    }
                    
                    case sf::Keyboard::Return:
                    {
                        switch (this->getSelectedItem())
                        {
                            case 0:
                                m_machine.lastState();
                                break;
                                
                            case 1:
                                m_next = StateMachine::build<OptionsState>( m_machine, m_window, false );
                                break;
                                
                            case 2:
                                // Add confirmation
                                m_next = StateMachine::build<MenuState>( m_machine, m_window, true );
                                break;
                        }
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

void PauseState::draw()
{
    // Clear previous drawing
    m_window.clear();
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    m_window.setView(view);
    m_window.draw( m_bg );
    m_window.draw( m_title );
    for (int i=0; i< m_PAUSE_ITEMS; i++) 
    {
        m_window.draw( m_menu[i] );
    }
    m_window.display();
}
