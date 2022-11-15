#include "mcrave_player.hpp"
#include "../../searchTrees/mcrave/mcrave.hpp"
namespace arena
{
    McRavePlayer::McRavePlayer(int n_game_actions, int n_sims, std::chrono::duration<int, std::milli> duration, float b)
        : n_game_actions_(n_game_actions),
          n_sims_(n_sims),
          duration_(duration),
          b_(b)

    {
    }

    int McRavePlayer::chooseAction(common::State *state_ptr)
    {
        searchTrees::Mcrave mcrave(n_game_actions_, b_);
        auto actions_probs = mcrave.search(state_ptr, n_sims_, duration_, 1.0f);

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
