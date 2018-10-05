#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

// include SFML and stack to manage game states
#include <SFML/Graphics.hpp>

#include <memory>
#include <stack>
#include <string>


class State;

namespace sf { class RenderWindow; }

class StateMachine
{
public:

    StateMachine();

    void run( std::unique_ptr<State> state );

    void nextState();
    void lastState();

    void update(const float dt);
    void draw();

    // Variable functions to check states
    bool running() { return m_running; }
    void quit() { m_running = false; }

    template <typename T>
    static std::unique_ptr<T> build( StateMachine& machine, sf::RenderWindow& window, bool replace = true );

private:

    std::stack<std::unique_ptr<State>> m_states; //State container

    bool m_resume;
    bool m_running;
};

template <typename T>
std::unique_ptr<T> StateMachine::build( StateMachine& machine, sf::RenderWindow& window, bool replace )
{
    return std::unique_ptr<T>( new T( machine, window, replace ));
}

#endif /* STATE_MACHINE_HPP */
