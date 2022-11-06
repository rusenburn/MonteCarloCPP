#ifndef ARENA_PLAYERS_MCTS_PLAYER_HPP_
#define ARENA_PLAYERS_MCTS_PLAYER_HPP_

#include <memory>
#include <chrono>
#include "../../common/player.hpp"
#include "../../common/evaluator.hpp"
namespace arena
{
    class MCTSPlayer : public common::Player
    {
    private:
        common::Evaluator *evaluator_ptr_;
        int n_game_actions_;
        int n_sims_;
        std::chrono::duration<int, std::milli> duration_;
        float temperature_;
        float cpuct_;

    public:
        MCTSPlayer(common::Evaluator *evaluator_ptr, int n_game_actions, int n_sims, std::chrono::duration<int, std::milli> duration, float temperature, float cpuct);
        MCTSPlayer(common::Evaluator *evaluator_ptr, int n_game_actions, int n_sims, float temperature, float cpuct)
            : MCTSPlayer(evaluator_ptr, n_game_actions, n_sims, std::chrono::duration<int, std::milli>(0), temperature, cpuct)
        {
        }

        MCTSPlayer(common::Evaluator *evaluator_ptr, int n_game_actions, std::chrono::duration<int, std::milli> duration, float temperature, float cpuct)
            : MCTSPlayer(evaluator_ptr, n_game_actions, 0, duration, temperature, cpuct)
        {
        }

        int chooseAction(common::State *state_ptr) override;
    };
} // namespace arena

#endif