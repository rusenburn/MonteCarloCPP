#ifndef COMMON_STATE_HPP_
#define COMMON_STATE_HPP_

#include <vector>
#include <tuple>
#include <memory>
#include <string>
#include "game_result.hpp"
#include "observation_space.hpp"
namespace common
{
    class State
    {
    public:
        // returns actions legal state vector
        virtual std::vector<int> getActionsLegality() = 0;

        // returns the shape of the state observation
        virtual common::ObservationSpace getObservationSpace() = 0;

        // returns the number of all actions in the State including illegal actions
        virtual int getActionsCount() = 0;

        // returns a bool indicating if the state reached a terminal state (ended)
        virtual bool isTerminal() = 0;

        // returns a vector representing {win,draw,loss} as a game result if game has ended else throw Exceptions
        virtual common::GameResult getResult() = 0;

        // performs an actions and returns a new state shared pointer
        virtual State *step(int action) = 0;

        // returns a vector representing the state
        virtual std::vector<int> getObservation() = 0;

        // returns a vector of symmetric states and action mapping vector
        virtual std::vector<std::tuple<State *, std::vector<int>>> getSymmerties() = 0;

        // returns a string representing the state , useful for maps
        virtual std::string getShort() = 0;

        // render the state
        virtual void render() = 0;

        virtual ~State(){};
    };
} // namespace common

#endif