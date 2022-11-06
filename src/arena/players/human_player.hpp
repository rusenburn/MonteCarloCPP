#ifndef ARENA_PLAYERS_HUMAN_PLAYER_HPP_
#define ARENA_PLAYERS_HUMAN_PLAYER_HPP_

#include "../../common/player.hpp"
#include "../../common/state.hpp"
namespace arena
{
    class HumanPlayer : public common::Player
    {
        public:
        int chooseAction(common::State* state_ptr)override;
    };
} // namespace players

#endif