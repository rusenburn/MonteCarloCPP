#include "grave_player.hpp"
#include "../../searchTrees/grave/grave.hpp"
namespace arena
{
    GravePlayer::GravePlayer(int n_game_actions, int n_sims, std::chrono::duration<int, std::milli> minimum_duration, int min_ref_count, float b_squared, bool save_illegal_amaf_actions)
        : n_game_actions_(n_game_actions),
          n_sims_(n_sims),
          minimum_duration_(minimum_duration),
          min_ref_count_(min_ref_count),
          b_sqaured_(b_squared),
          save_illegal_amaf_actions_(save_illegal_amaf_actions)
    {
    }

    int GravePlayer::chooseAction(common::State *state_ptr)
    {
        searchTrees::Grave grave(n_game_actions_, min_ref_count_, b_sqaured_, save_illegal_amaf_actions_);

        auto actions_probs = grave.search(state_ptr, n_sims_, minimum_duration_, 1.0f);

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
