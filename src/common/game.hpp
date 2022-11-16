#ifndef COMMON_GAME_HPP_
#define COMMON_GAME_HPP_

#include "state.hpp"

namespace common
{
    class Game
    {
    public:
        // returns the number of all actions in the Game including illegal actions
        virtual int getActionsCount() = 0;

        // returns the shape of the game observation
        virtual common::ObservationSpace getObservationSpace() = 0;

        // resets the game , returning a new state pointer
        virtual State *reset() = 0;

        // performing action in the game returning a new state pointer
        virtual State *step(int action) = 0;

        // checks if the game is over , return true if it is ,else return false.
        virtual bool isTerminal() = 0;

        // render the game
        virtual void render() = 0;
        virtual ~Game(){};
    };
} // namespace common

#endif