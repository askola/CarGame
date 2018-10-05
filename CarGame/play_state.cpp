#include "play_state.hpp"
#include "pause_state.hpp"
#include "game_over_state.hpp"
#include "state_machine.hpp"
#include "track/obstacles.hpp"
#include "track/map.hpp"

// SFML headers
#include <SFML/Window/Event.hpp>
#include <string>
#include <iostream> // For debugging
#include <math.h>

PlayState::PlayState(StateMachine& machine, sf::RenderWindow& window, bool replace)
: State {
    machine, window, replace
}
{
    std::cout << "PlayState Init" << std::endl;

    if (!m_roadTexture.loadFromFile("track/Images/countryroad.jpg"))
        std::cerr << "Error loading countryroad.jpg" << std::endl;
    m_roadTexture.setSmooth(true);

    if (!m_grassTexture.loadFromFile("track/Images/grass.png"))
        std::cerr << "Error loading grass.png" << std::endl;

    if (!m_finishlineTexture.loadFromFile("track/Images/finishline.jpg"))
        std::cerr << "Error loading finishline.jpg" << std::endl;
        
    if(!m_obstacleTexture.loadFromFile("img/tire.png"))
        std::cerr << "Error loading texture tire.png" << std::endl;

    // Set font
    if (!m_font.loadFromFile("./fonts/justice.ttf"))
        std::cerr << "Error loading font justice.ttf" << std::endl;

    m_start_text.setFont(m_font);
    m_start_text.setCharacterSize(80);
    m_start_text.setColor(sf::Color::Blue);

    m_lapTimer.setFont(m_font);
    m_lapTimer.setCharacterSize(60);
    m_lapTimer.setColor(sf::Color::White);
    m_lapTimer.setPosition(sf::Vector2f(20.f, 20.f));
    m_timer.setFont(m_font);
    m_timer.setCharacterSize(30);
    m_timer.setColor(sf::Color::Black);
    m_timer.setPosition(sf::Vector2f(20.f, 80.f));

    m_world = new b2World(b2Vec2(0, 0), true);
    std::string mapname = "track/1mappoints.txt"; //LATAA FILUSTA!!!!
    m_map->createWorld(m_world, mapname);
    Obstacles *obstacles = new Obstacles();

    std::vector<std::pair<float, float>> car = m_map->CarVector(mapname);
    for (unsigned int i = 0; i < car.size(); i++) {
        float x = car[i].first;
        float y = car[i].second;
        Cars.push_back(new Vehicle(m_world, i, x, y));
    }

    std::vector<std::pair<float, float>> solid = m_map->SolidVector(mapname);
    for (unsigned int i = 0; i < solid.size(); i++) {
        float x = solid[i].first;
        float y = solid[i].second;
        sf::CircleShape tire = obstacles->solid(x, y, 2, m_world);
        tire.setTexture(&m_obstacleTexture,true);
        m_objects.push_back(tire);
    }

    std::vector<std::pair<float, float>> oil = m_map->OilVector(mapname);
    for (unsigned int i = 0; i < oil.size(); i++) {
        float x = oil[i].first;
        float y = oil[i].second;
        sf::CircleShape oilSpill = obstacles->oil(x, y, 3.5f, m_world);
        m_objects.push_back(oilSpill);
    }

    for (auto i = 0; i < 16; i++) {
        sf::ConvexShape convout = m_map->convout(i, mapname);
        convout.setTexture(&m_grassTexture, true);
        m_convexShapes.push_back(convout);
    }

    std::vector<std::pair<float, float>> roadvec = m_map->RoadVector(mapname);
    for (unsigned int i = 0; i < roadvec.size(); i++) {
        sf::ConvexShape convroad = m_map->convroad(i, mapname);
        convroad.setTexture(&m_roadTexture, true);
        m_convexShapes.push_back(convroad);
    }

    for (unsigned int i = 0; i < roadvec.size(); i++) {
        sf::ConvexShape convin = m_map->convin(i, mapname);
        convin.setTexture(&m_grassTexture, true);
        m_convexShapes.push_back(convin);
    }

    // Create finish line
    
    m_finishlineTexture.setRepeated(true);
    m_finishline = m_map->finishline(mapname);
    m_rectangle.setTextureRect(sf::IntRect(m_finishline.getPosition().x, m_finishline.getPosition().y, m_finishline.getSize().x, m_finishline.getSize().y));
    m_rectangle.setRotation(m_finishline.getRotation());
    m_finishline.setTexture(&m_finishlineTexture, true);
}

void PlayState::pause() {
    std::cout << "PlayState Pause" << std::endl;
}

void PlayState::resume() {
    std::cout << "PlayState Resume" << std::endl;
}

void PlayState::update(const float dt) {
    if (!m_raceCntDwn) {
        if (m_raceInit) {
            m_clock.restart();
            for(auto i : Cars){
            
                i->getLapClock().restart();
            
            }
            
            m_raceInit = false;
        }




        // Format the timers to:
        //      Lap 1 : 00:000
        //      Total : 00:00
        
        m_raceTime = m_clock.getElapsedTime();
        m_raceMins = std::to_string((int) m_raceTime.asSeconds() % 60);
        m_raceSecs = std::to_string(m_raceTime.asMilliseconds() % 1000);
        m_raceFormat = "Total : " + m_raceMins + ":" + m_raceSecs;

        
        m_timer.setString(m_raceFormat);
        
        sf::Event m_event;

        while (m_window.pollEvent(m_event)) {
            switch (m_event.type) {

                case sf::Event::Closed:
                {
                    m_machine.quit();
                    break;
                }

                default:
                    break;
            }
        }
        if(Cars[0]->getLap() == 4){
            m_next = StateMachine::build<GameOverState>(m_machine, m_window, true);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            m_next = StateMachine::build<PauseState>(m_machine, m_window, false);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            Cars[0]->throttle();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            Cars[0]->turnLeft();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            Cars[0]->brake();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            Cars[0]->turnRight();
        }
    }

    if (m_raceCntDwn) {

        m_lapTimer.setString("Lap 1 : 00:000");
        m_timer.setString("Total : 00:00'000");

        if (m_counter == 0 && m_clock.getElapsedTime().asSeconds() > 0.5f) {
            std::cout << "Get Ready To Race" << std::endl;
            m_start_text.setString("Get Ready To Race");
            m_counter++;
        }

        if (m_counter == 1 && m_clock.getElapsedTime().asSeconds() > 2.5f) {
            std::cout << "3" << std::endl;
            m_start_text.setString("3");
            m_counter++;
        }

        if (m_counter == 2 && m_clock.getElapsedTime().asSeconds() > 3.5f) {
            std::cout << "2" << std::endl;
            m_start_text.setString("2");
            m_counter++;
        }

        if (m_counter == 3 && m_clock.getElapsedTime().asSeconds() > 4.5f) {
            std::cout << "1" << std::endl;
            m_start_text.setString("1");
            m_counter++;
        }

        if (m_counter == 4 && m_clock.getElapsedTime().asSeconds() > 5.5f) {
            std::cout << "GO" << std::endl;
            m_start_text.setString("GO");
            m_counter++;
        }

        if (m_counter == 5 && m_clock.getElapsedTime().asSeconds() > 6.5f) {
            m_raceCntDwn = false;
            m_counter = 0;
        }

        sf::FloatRect m_textRect = m_start_text.getLocalBounds();
        m_start_text.setOrigin(m_textRect.left + m_textRect.width / 2.0f,
                m_textRect.top + m_textRect.height / 2.0f);
        m_start_text.setPosition(sf::Vector2f(m_window.getSize().x / 2.0f, m_window.getSize().y / 2.5f));
    }

    for (auto it = Cars.begin(); it != Cars.end(); it++) {

        (*it)->drag();
        (*it)->updateFriction();
        sf::Sprite& Sprite = (*it)->getSprite();
        sf::Text& Text = (*it)->getText();
        b2Body* Body = (*it)->getBody();
        Sprite.setPosition(SCALE * Body->GetPosition().x, SCALE * Body->GetPosition().y);
        Sprite.setRotation(Body->GetAngle() * RADTODEG);
        Sprite.setOrigin(sf::Vector2f(200.f, 400.f));
        Text.setOrigin(sf::Vector2f(200.f, 400.f));
        Text.setPosition(Sprite.getPosition());
        (*it)->hitOil();
        if((*it)->getOil()){
            (*it)->updateEffectTime(dt);
            (*it)->looseOil();
        }
        // Checkpoint & Laps
        if((*it)->checkpoints() == 1 && (*it)->getLastCheck() == 4){
            
            (*it)->setLap();
            (*it)->setLastCheck(1);
            (*it)->increaseLap();
            (*it)->getLapClock().restart();
        
        }
        
        if((*it)->checkpoints() == 2 && (*it)->getLastCheck() == 1){
        
            (*it)->setLastCheck(2);
        
        }
        
        if((*it)->checkpoints() == 3 && (*it)->getLastCheck() == 2){
        
            (*it)->setLastCheck(3);
        
        }
        
        
        if((*it)->checkpoints() == 4 && (*it)->getLastCheck() == 3){
        
            (*it)->setLastCheck(4);
        
        }
    }
    
    m_world->Step(dt, 6, 6);
}

void PlayState::draw() {

    // Clear previous drawing
    m_window.clear();
    
    sf::View view(sf::Vector2f(SCALE * Cars[0]->getBody()->GetPosition().x, SCALE * Cars[0]->getBody()->GetPosition().y), sf::Vector2f(1200, 900));
    m_window.setView(view);
    

    //    if ( m_raceCntDwn ) {
    //        m_window.draw(m_start_text);
    //    }

    for (auto i : m_convexShapes) {
        m_window.draw(i);
    }

    m_window.draw(m_finishline);
    
    for ( auto i : m_objects) {
        m_window.draw(i);
    }

    for (auto i : Cars) {
        m_window.draw(i->getSprite());
    }
    
    //Draw Mini Map
    sf::View miniMap(sf::FloatRect(-5000, -3700, 7000, 7000));
    miniMap.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f));
    m_window.setView(miniMap);

    for (auto i : m_convexShapes) {
        m_window.draw(i);
    }
    
    m_window.draw(m_finishline);
    
    for ( auto i : m_objects) {
        m_window.draw(i);
    }

    for (auto i : Cars) {
        m_window.draw(i->getText());
    }

    //Draw Start count and race&lap-timers
    sf::View timers(sf::FloatRect(0, 0, m_window.getSize().x, m_window.getSize().y));
    m_window.setView(timers);

    if (m_raceCntDwn) {
        m_window.draw(m_start_text);
    }
    m_window.draw(m_timer);
    m_lapTimer.setString(Cars[0]->getLapFormat());
    m_window.draw(m_lapTimer);

    m_window.display();
}
