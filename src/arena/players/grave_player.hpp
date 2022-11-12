#ifndef ARENA_PLAYERS_GRAVE_PLAYER_HPP_
#define ARENA_PLAYERS_GRAVE_PLAYER_HPP_

#include <chrono>
#include "../../common/player.hpp"
namespace arena
{
    class GravePlayer : public common::Player
    {
    private:
        int n_game_actions_;
        int n_sims_;
        std::chrono::duration<int, std::milli> minimum_duration_;
        int min_ref_count_;
        float b_sqaured_;
        bool save_illegal_amaf_actions_;

    public:
        GravePlayer(int n_game_actions, int n_sims, std::chrono::duration<int, std::milli> duration, int min_ref_count, float b_squared, bool save_illegal_amaf_actions);
        int chooseAction(common::State *state_ptr) override;
    };
} // namespace arena

#endif
