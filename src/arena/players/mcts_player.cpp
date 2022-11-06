#include <vector>
#include <random>
#include "mcts_player.hpp"
#include "../../common/evaluator.hpp"
#include "../../searchTrees/monteCarlo/mcts.hpp"

namespace arena
{
    MCTSPlayer::MCTSPlayer(common::Evaluator* evaluator_ptr, int n_game_actions, int n_sims, std::chrono::duration<int, std::milli> duration, float temperature, float cpuct)
        : evaluator_ptr_(evaluator_ptr),
          n_game_actions_(n_game_actions),
          n_sims_(n_sims),
          duration_(duration),
          temperature_(temperature),
          cpuct_(cpuct)
    {
    }

    int MCTSPlayer::chooseAction(common::State* state_ptr)
    {
        auto mcts = searchTrees::MCTS(n_game_actions_,evaluator_ptr_,cpuct_);
        std::vector<float> actions_probs = mcts.search(state_ptr,n_sims_,duration_,temperature_);

        float p = rand() / static_cast<float>(RAND_MAX + 1);

        float remaining_prob = p;

        int action = 0;

        // ex: if we have 10 actions then the last action is 9 including 0;
        int last_action = n_game_actions_ - 1;

        // keep decreasing remaining probs until it is below zero or only 1 action remains
        while ((action < last_action) && ((remaining_prob -= actions_probs[action]) >= 0))
        {
            action++;
        }

        return action;
    }
} // namespace arena
