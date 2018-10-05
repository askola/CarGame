//Include headers
#include "state_machine.hpp"
#include "state.hpp"

//Includes for functions
#include <iostream> // Debugging
#include <memory>

StateMachine::StateMachine() : m_resume(false), m_running(false)
{
    std::cout << "StateMachine Init" <<  std::endl;
}

void StateMachine::run( std::unique_ptr<State> state )
{
    m_running = true;
    m_states.push( std::move(state) );
}

void StateMachine::nextState()
{
    if (m_resume)
    {
        // When stack not empty move back to prev state
        if ( !m_states.empty() )
            m_states.pop();

        if ( !m_states.empty() )
            m_states.top()->resume();

        // Set resume flag back to false
        m_resume = false;
    }

    // Avoid running out of states

    if ( !m_states.empty() )
    {
        std::unique_ptr<State> temp = m_states.top()->next();

        // Only change if other states exist
        if (temp != nullptr)
        {
            // Replace the current state
            if ( temp->isReplacing() )
                m_states.pop();

            // Pause the current state
            else
                m_states.top()->pause();

            m_states.push( std::move( temp ) );
        }
    }
}

void StateMachine::lastState()
{
    m_resume = true;
}

void StateMachine::update(const float dt)
{
    // Allow current state to update the game
    m_states.top()->update(dt);
}

void StateMachine::draw()
{
    // Allow current state to draw to the screen
    m_states.top()->draw();
}
