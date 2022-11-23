#include "connect4_game.hpp"

namespace games
{
    Connect4Game::Connect4Game()
        : state_ptr(getNewState())
    {
    }

    int Connect4Game::getActionsCount()
    {
        return state_ptr->getActionsCount();
    }

    common::ObservationSpace Connect4Game::getObservationSpace()
    {
        return state_ptr->getObservationSpace();
    }

    Connect4State *Connect4Game::step(int action)
    {
        state_ptr = state_ptr->step(action);
        return state_ptr;
    }

    Connect4State *Connect4Game::reset()
    {
        state_ptr = getNewState();
        return state_ptr;
    }

    void Connect4Game::render()
    {
        state_ptr->render();
    }

    bool Connect4Game::isTerminal()
    {
        return state_ptr->isTerminal();
    }

    Connect4State *Connect4Game::getNewState()
    {
        std::array<std::array<int, 7>, 6> new_obs;
        for (auto &row : new_obs)
        {
            row.fill(0);
        }
        int last_action = -1;
        int turn = 0;
        Connect4State *new_state_ptr = new Connect4State(new_obs, last_action, turn);
        return new_state_ptr;
    }

    Connect4Game::~Connect4Game(){};

} // namespace games
