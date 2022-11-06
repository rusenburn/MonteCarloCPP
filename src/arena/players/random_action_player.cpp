#include <random>
#include "random_action_player.hpp"

namespace arena
{
    RandomActionPlayer::RandomActionPlayer(int n_game_actions)
        : n_game_actions_(n_game_actions)
    {}

    int RandomActionPlayer::chooseAction(common::State* state_ptr)
    {
        std::vector<int> actions_legality = state_ptr->getActionsLegality();
        std::vector<int> legal_actions{};
        int legal_actions_count = 0;

        for (int action = 0; action < n_game_actions_; action++)
        {
            if (actions_legality[action] != 0)
            {
                legal_actions.push_back(action);
            }
        }

        int action_idx = rand() % legal_actions.size();
        int action = legal_actions[action_idx];
        return action;
    }
} // namespace arena
