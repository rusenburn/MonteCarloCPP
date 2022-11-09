#ifndef ARENA_PLAYERS_UCT_PLAYER_HPP_
#define ARENA_PLAYERS_UCT_PLAYER_HPP_

#include <chrono>
#include "../../common/player.hpp"
namespace arena
{
    class UctPlayer : public common::Player
    {
    private:
        int n_game_actions_;
        int n_sims_;
        std::chrono::duration<int, std::milli> duration_;
        float temperature_;
        float cuct_;

    public:
        UctPlayer(int n_game_actions, int n_sims, std::chrono::duration<int, std::milli> duration, float temperature, float cuct);
        int chooseAction(common::State *state_ptr);
    };
} // namespace arena

#endif