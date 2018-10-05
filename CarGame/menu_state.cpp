#include "menu_state.hpp"
#include "game_mode_select.hpp"
#include "options_state.hpp"
#include "state_machine.hpp"

#include <iostream> // For debugging
#include <memory>

MenuState::MenuState( StateMachine& machine, sf::RenderWindow& window, bool replace )
: State{ machine, window, replace }
{
    std::cout << "MenuState Init" << std::endl;
   
    // Set background
    if(!m_bgTex.loadFromFile( "./img/menu.png" ))
        std::cerr << "Error loading menu.png" << std::endl;
    
    m_bg.setTexture( m_bgTex, true );
    
    // Set font
    if(!m_font.loadFromFile("./fonts/justice.ttf"))
        std::cerr << "Error loading font justice.ttf" << std::endl;
    if(!m_font_shadow.loadFromFile("./fonts/justice3d.ttf"))
        std::cerr << "Error loading font justice3d.ttf" << std::endl;
    
    // Set Menu items
    m_menu[0].setFont(m_font_shadow);
    m_menu[0].setCharacterSize(80);
    m_menu[0].setString("Play");
    m_menu[0].setPosition(sf::Vector2f( 60.f, (window.getSize().y - ( 60.f + ( 80.f ) * m_MENU_ITEMS)) ));
    m_menu[0].setColor( sf::Color::Black );
    
    m_menu[1].setFont(m_font);
    m_menu[1].setCharacterSize(80);
    m_menu[1].setString("Controls");
    m_menu[1].setPosition(sf::Vector2f( 60.f, ( window.getSize().y - (60.f+(80.f)*(m_MENU_ITEMS-1))) ));
    m_menu[1].setColor( sf::Color::White );
    
    m_menu[2].setFont(m_font);
    m_menu[2].setCharacterSize(80);
    m_menu[2].setString("Exit");
    m_menu[2].setPosition(sf::Vector2f( 60.f, ( window.getSize().y - (60.f+(80.f)*(m_MENU_ITEMS-2))) ));
    m_menu[2].setColor( sf::Color::White );

    // Init highlight
    m_selectedItemIndex = 0;
    
}

void MenuState::moveUp()
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

void MenuState::moveDown()
{
    if ( m_selectedItemIndex+1 < m_MENU_ITEMS )
    {
        m_menu[m_selectedItemIndex].setColor( sf::Color::White );
        m_menu[m_selectedItemIndex].setFont(m_font);
        
        m_selectedItemIndex++;
        
        m_menu[m_selectedItemIndex].setColor( sf::Color::Black );
        m_menu[m_selectedItemIndex].setFont(m_font_shadow);
    }
}

void MenuState::pause()
{
    std::cout << "MenuState Pause" << std::endl;
}

void MenuState::resume()
{
    std::cout << "MenuState Resume" << std::endl;
}

void MenuState::update(const float dt)
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
                                m_next = StateMachine::build<GameModeSelect>( m_machine, m_window, false );
                                break;
                                
                            case 1:
                                m_next = StateMachine::build<OptionsState>( m_machine, m_window, false );
                                break;
                                
                            case 2:
                                m_machine.quit();
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

void MenuState::draw()
{
    // Clear previous drawing
    m_window.clear();
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    m_window.setView(view);    
    m_window.draw( m_bg );
    for (int i=0; i< m_MENU_ITEMS; i++) 
    {
        m_window.draw( m_menu[i] );
    }
    m_window.display();
}
