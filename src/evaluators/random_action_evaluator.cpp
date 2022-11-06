#include "random_action_evaluator.hpp"

#include <vector>
namespace evaluators
{
    common::Evaluation RandomActionEvaluator::evaluate(common::State *state_ptr)
    {
        if (state_ptr->isTerminal())
        {
            throw "state is over exception";
        }

        int n_actions = state_ptr->getActionsCount();
        int player = 0;
        std::vector<int> actions_legality = state_ptr->getActionsLegality();
        int legal_actions_count;
        for (int action = 0; action < n_actions; action++)
        {
            legal_actions_count += actions_legality[action];
        }

        std::vector<float> actions_probs(n_actions, 0.0f);
        std::vector<int> legal_actions{};
        actions_probs.reserve(n_actions);
        for (int action = 0; action < n_actions; action++)
        {
            if (actions_legality[action] == 1)
            {
                legal_actions.push_back(action);
            }
        }
        legal_actions_count = legal_actions.size();
        for (auto &action : legal_actions)
        {
            actions_probs[action] = 1.0f / legal_actions_count;
        }

        int action_idx = rand() % legal_actions_count;
        int action = legal_actions[action_idx];

        state_ptr = state_ptr->step(action);
        player = 1 - player;

        while (!state_ptr->isTerminal())
        {
            actions_legality = state_ptr->getActionsLegality();
            legal_actions_count = 0;
            std::vector<int> legal_actions;
            for (int i = 0; i < n_actions; i++)
            {
                if (actions_legality[i] == 1)
                {
                    legal_actions.push_back(i);
                }
            }
            legal_actions_count = legal_actions.size();

            int action_idx = rand() % legal_actions_count;
            int action = legal_actions[action_idx];

            common::State *new_state_ptr = state_ptr->step(action);
            delete state_ptr;
            state_ptr = new_state_ptr;
            player = 1 - player;
        }

        std::vector<float> wdl = {0.0f, 0.0f, 0.0f};
        common::GameResult result = state_ptr->getResult();
        if (result == common::GameResult::win)
        {
            wdl[0] = 1.0f;
        }
        else if (result == common::GameResult::draw)
        {
            wdl[1] = 1.0f;
        }
        else
        {
            wdl[2] = 1.0f;
        }

        if (player == 1)
        {
            // reverse it;
            float temp = wdl[0];
            wdl[0] = wdl[2];
            wdl[2] = temp;
        }

        delete state_ptr;
        return {actions_probs, wdl};
    }
} // namespace evaluators
