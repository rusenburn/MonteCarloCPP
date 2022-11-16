#ifndef COMMON_PLAYER_HPP_
#define COMMON_PLAYER_HPP_

#include <memory>
#include "state.hpp"

namespace common
{
    class Player
    {
    public:
        virtual int chooseAction(State *state_ptr) = 0;
        virtual ~Player(){}
    };
} // namespace common

#endif