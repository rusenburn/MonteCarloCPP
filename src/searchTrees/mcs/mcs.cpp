#include <algorithm>
#include <vector>
#include <chrono>
#include <math.h>
#include "../../common/state.hpp"
#include "mcs.hpp"

namespace searchTrees
{
    MCS::MCS(int n_game_actions, float cuct)
        : n_game_actions_(n_game_actions),
          cuct_(cuct)
    {
    }

    std::vector<float> MCS::search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature)
    {
        Node node(state_ptr, n_game_actions_, cuct_, temperature);
        std::vector<float> actions_probs(n_game_actions_);
        node.search(simulation_count, duration, actions_probs);
        return actions_probs;
    }

    MCS::Node::Node(common::State *state_ptr, int n_game_actions, float cuct, float temperature)
        : state_ptr_(state_ptr),
          n_game_actions_(n_game_actions),
          cuct_(cuct),
          temperature_(temperature),
          actions_legality_(state_ptr->getActionsLegality()),
          n_(0),
          na_(std::vector<int>(n_game_actions)),
          nwa_(std::vector<float>(n_game_actions)),
          nda_(std::vector<float>(n_game_actions)),
          nla_(std::vector<float>(n_game_actions)) {}

    void MCS::Node::search(int minimum_simulations_count, std::chrono::duration<int, std::milli> minimum_duration, std::vector<float> &out_actions_probs)
    {
        if (state_ptr_->isTerminal())
        {
            throw "State is game over";
        }

        auto t_end = std::chrono::high_resolution_clock::now() + minimum_duration;

        for (int i = 0; i < minimum_simulations_count; i++)
        {
            simulateOne();
        }

        while (t_end > std::chrono::high_resolution_clock::now())
        {
            simulateOne();
        }

        getFinalProbabilities(out_actions_probs);
    }

    void MCS::Node::simulateOne()
    {
        int best_action = getBestAction();
        std::vector<float> wdl = {0.0f, 0.0f, 0.0f};
        rollout(best_action, wdl);

        n_++;
        na_[best_action]++;
        nwa_[best_action] += wdl[0];
        nda_[best_action] += wdl[1];
        nla_[best_action] += wdl[2];
    }

    void MCS::Node::rollout(int action, std::vector<float> &out_wdl)
    {
        common::State *rollout_state_ptr = state_ptr_->step(action);
        int depth = 1;
        while (!rollout_state_ptr->isTerminal())
        {
            std::vector<int> rollout_actions_legality = rollout_state_ptr->getActionsLegality();

            int n_legal_actions = 0;
            for (int i = 0; i < n_game_actions_; i++)
            {
                n_legal_actions += rollout_actions_legality[i];
            }

            int random_legal_action_idx = rand() % n_legal_actions;
            int random_action = -1;
            while (random_legal_action_idx >= 0)
            {
                if (rollout_actions_legality[++random_action] == 1)
                    random_legal_action_idx--;
            }
            common::State *new_state = rollout_state_ptr->step(random_action);
            delete rollout_state_ptr;
            rollout_state_ptr = new_state;
            depth++;
        }
        common::GameResult result = rollout_state_ptr->getResult();
        if (result == common::GameResult::win)
        {
            out_wdl[0] = 1.0f;
        }
        else if (result == common::GameResult::draw)
        {
            out_wdl[1] = 1.0f;
        }
        else // common::GameResult::loss
        {
            out_wdl[2] = 1.0f;
        }
        if (depth % 2 != 0)
        {
            float temp = out_wdl[0];
            out_wdl[0] = out_wdl[2];
            out_wdl[2] = temp;
        }
        delete rollout_state_ptr;
    }

    void MCS::Node::getFinalProbabilities(std::vector<float> &out_action_probs)
    {
        if (temperature_ == 0)
        {
            int max_visits_count = *std::max_element(na_.begin(), na_.end());

            std::vector<int> best_actions{};
            for (int action = 0; action < n_game_actions_; action++)
            {
                if (na_[action] == max_visits_count)
                {
                    best_actions.push_back(action);
                }
            }

            int best_action_idx = rand() % best_actions.size();
            int best_action = best_actions[best_action_idx];

            for (int action = 0; action < n_game_actions_; action++)
            {
                out_action_probs[action] = action == best_action ? 1.0f : 0.0f;
            }
            return;
        }

        float sum_probs = 0;
        for (int action = 0; action < n_game_actions_; action++)
        {
            float prob = float(na_[action]) / n_;
            float new_prob = powf(prob, 1.0f / temperature_);
            sum_probs += new_prob;
            out_action_probs[action] = new_prob;
        }

        for (int action = 0; action < n_game_actions_; action++)
        {
            out_action_probs[action] /= sum_probs;
        }
    }

    int MCS::Node::getBestAction()
    {
        if (actions_legality_.size() == 0)
        {
            throw "Exception actions legality should not be 0";
        }

        float max_u = -std::numeric_limits<float>::infinity();
        int best_action = -1;
        for (int action = 0; action < n_game_actions_; action++)
        {
            if (actions_legality_[action] == 0)
            {
                continue;
            }

            float na = float(na_[action]);

            // Q(s,a)
            float qsa = 0;

            if (na > 0)
            {
                float nwa = nwa_[action];
                float nla = nla_[action];
                qsa = (nwa - nla) / na;
            }

            float u;

            if (na == 0)
            {
                u = std::numeric_limits<float>::infinity();
            }
            else
            {
                u = qsa + cuct_ * sqrtf(logf(float(n_)) / na);
            }

            if (u > max_u)
            {
                max_u = u;
                best_action = action;
            }
        }

        if (best_action != -1)
        {
            return best_action;
        }
        // Should not reach this code unless something went wrong
        // Pick random action instead
        std::vector<int> best_actions{};
        for (int action=0; action < n_game_actions_; action++)
        {
            if (actions_legality_[action] == 1)
            {
                best_actions.push_back(action);
            }
        }

        best_action = rand() % best_actions.size();
        return best_actions[best_action];
    }
} // namespace searchTrees
