#ifndef ARENA_MCRAVE_PLAYER_HPP_
#define ARENA_MCRAVE_PLAYER_HPP_

#include <chrono>
#include "../../common/player.hpp"
namespace arena
{
    class McRavePlayer : public common::Player
    {
        private:
        int n_game_actions_;
        int n_sims_;
        std::chrono::duration<int,std::milli> duration_;
        float b_;
        public:
        McRavePlayer(int n_game_actions,int n_sims,std::chrono::duration<int,std::milli> duration,float b);
        int chooseAction(common::State* state_ptr);
    };
} // arena

#endif