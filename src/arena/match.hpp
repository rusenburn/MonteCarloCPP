#ifndef ARENA_MATCH_HPP_
#define ARENA_MATCH_HPP_

#include <memory>
#include "../common/game.hpp"
#include "../common/player.hpp"
namespace arena
{
    class Match
    {
    private:
        std::shared_ptr<common::Game> game_ptr_;
        common::Player *player1_ptr_;
        common::Player *player2_ptr_;
        int n_sets_;
        bool render_;
        std::vector<int> scores_;
        std::vector<int> play_set(int starting_player);

    public:
        Match(std::shared_ptr<common::Game> (*game_fn_ptr)(), common::Player *player_1_ptr, common::Player *player_2_ptr, int n_sets, bool render);
        std::vector<int> start();
    };
} // namespace arena

#endif