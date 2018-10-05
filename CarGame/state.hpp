#ifndef STATE_HPP
#define STATE_HPP

#include <memory>

/*
To define a gamestate.
All the Games functions are pure virtual.
*/

class StateMachine;
namespace sf { class RenderWindow; }

class State
{
public:

    State( StateMachine& machine, sf::RenderWindow& window, bool replace = true );
    virtual ~State() = default;

    State ( const State& ) = delete;
    State& operator= ( const State& ) = delete;

    virtual void pause(  ) = 0;
    virtual void resume(  ) = 0;

    virtual void update( const float dt ) = 0;
    virtual void draw() = 0;

    std::unique_ptr<State> next();

    bool isReplacing();

protected:

    StateMachine& m_machine;
    sf::RenderWindow& m_window;

    bool m_replacing;

    std::unique_ptr<State> m_next;
};

#endif // STATE_HPP

