#ifndef ARENA_PLAYERS_RANDOM_ACTION_PLAYER_HPP_
#define ARENA_PLAYERS_RANDOM_ACTION_PLAYER_HPP_

#include <memory>

#include "../../common/state.hpp"
#include "../../common/player.hpp"

namespace arena
{
    class RandomActionPlayer : public common::Player
    {
    private:
        int n_game_actions_;

    public:
        RandomActionPlayer(int n_game_actions);
        int chooseAction(common::State *state_ptr) override;
    };
} // namespace arena

#endif