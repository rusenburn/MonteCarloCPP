#include "mcs_player.hpp"
#include "../../searchTrees/mcs/mcs.hpp"
namespace arena
{
    MCSPlayer::MCSPlayer(int n_game_actions, int n_sims, std::chrono::duration<int, std::milli> duration, float temperature, float cuct)
        : n_game_actions_(n_game_actions),
          n_sims_(n_sims),
          duration_(duration),
          temperature_(temperature),
          cuct_(cuct)
    {
    }

    int MCSPlayer::chooseAction(common::State *state_ptr)
    {
        searchTrees::MCS mcs = searchTrees::MCS(n_game_actions_, cuct_);
        std::vector<float> actions_probs = mcs.search(state_ptr, n_sims_, duration_, temperature_);

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
