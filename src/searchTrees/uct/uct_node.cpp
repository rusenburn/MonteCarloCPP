#include <algorithm>
#include <math.h>
#include "uct_node.hpp"
#include "../../common/terminal_status.hpp"
namespace searchTrees
{
    UctNode::UctNode(common::State *state_ptr, int n_game_actions, float cuct)
        : state_ptr_(state_ptr),
          n_game_actions_(n_game_actions),
          cuct_(cuct),
          terminal_(common::TerminalStatus::Unknown),
          is_game_result_cached_(false),
          game_result_(common::GameResult::loss),
          actions_legality_(std::vector<int>()),
          children_(n_game_actions, nullptr),
          n_(0),
          qsa_(n_game_actions),
          nsa_(n_game_actions)
    {
    }

    void UctNode::search(int minimum_simulations, std::chrono::duration<int, std::milli> minimum_duration, float uct, float temperature, std::vector<float> &out_actions_probs)
    {
        if (terminal_ == common::TerminalStatus::Unknown)
        {
            terminal_ = state_ptr_->isTerminal() ? common::TerminalStatus::Done : common::TerminalStatus::NotYet;
        }
        if (terminal_ == common::TerminalStatus::Done)
        {
            throw "State is terminal exception";
        }

        auto t_end = std::chrono::high_resolution_clock::now() + minimum_duration;
        for (int i = 0; i < minimum_simulations; i++)
        {
            simulateOne();
        }

        while (t_end > std::chrono::high_resolution_clock::now())
        {
            simulateOne();
        }

        getFinalProbabilities(temperature, out_actions_probs);
    }

    float UctNode::simulateOne()
    {
        if (terminal_ == common::TerminalStatus::Unknown)
        {
            terminal_ = state_ptr_->isTerminal() ? common::TerminalStatus::Done : common::TerminalStatus::NotYet;
        }
        if (terminal_ == common::TerminalStatus::Done)
        {
            if (!is_game_result_cached_)
            {
                game_result_ = state_ptr_->getResult();
            }
            // reverse it
            if (game_result_ == common::GameResult::win)
            {
                return -1.0f;
            }
            else if (game_result_ == common::GameResult::loss)
            {
                return 1.0f;
            }
            else
                return 0.0f;
        }

        if (actions_legality_.size() == 0) // if size is 0 then this is the first none terminal visit
        {
            actions_legality_ = state_ptr_->getActionsLegality();
            float z = rollout(state_ptr_);

            return -z;
        }

        int best_action = findBestAction();

        if (children_[best_action] == nullptr)
        {
            common::State *new_state_ptr = state_ptr_->step(best_action);
            UctNode *new_node_ptr = new UctNode(new_state_ptr, n_game_actions_, cuct_);
            children_[best_action] = new_node_ptr;
        }

        UctNode *new_node_ptr = children_[best_action];
        float z = new_node_ptr->simulateOne();
        n_++;
        nsa_[best_action]++;
        qsa_[best_action] += (z - qsa_[best_action]) / float(nsa_[best_action]);
        return -z;
    }

    int UctNode::findBestAction()
    {
        if (actions_legality_.size() == 0)
        {
            throw "Exception actions legality size should not be 0";
        }

        float max_u = -std::numeric_limits<float>::infinity();
        int best_action = -1;

        for (int action = 0; action < n_game_actions_; action++)
        {
            if (actions_legality_[action] == 0)
            {
                continue;
            }

            float qsa = qsa_[action];
            float nsa = nsa_[action];
            float u;
            if (nsa == 0)
            {
                u = std::numeric_limits<float>::infinity();
            }
            else
            {
                u = qsa_[action] + cuct_ * sqrt(log(float(n_)) / nsa);
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
        for (int action; action < n_game_actions_; action++)
        {
            if (actions_legality_[action] == 1)
            {
                best_actions.push_back(action);
            }
        }

        best_action = rand() % best_actions.size();
        return best_actions[best_action];
    }

    void UctNode::getFinalProbabilities(float temperature, std::vector<float> &out_actions_probs)
    {
        if (temperature == 0.0f)
        {
            int max_visits_count = *std::max_element(nsa_.begin(), nsa_.end());

            std::vector<int> best_actions{};
            for (int action = 0; action < n_game_actions_; action++)
            {
                if (nsa_[action] == max_visits_count)
                {
                    best_actions.push_back(action);
                }
            }

            int best_action_idx = rand() % best_actions.size();
            int best_action = best_actions[best_action_idx];

            for (int action = 0; action < n_game_actions_; action++)
            {
                out_actions_probs[action] = action == best_action ? 1 : 0;
            }
            return;
        }

        float sum_probs = 0;
        for (int action = 0; action < n_game_actions_; action++)
        {
            float prob = float(nsa_[action]) / n_;
            float new_prob = pow(prob, 1.0f / temperature);
            sum_probs += new_prob;
            out_actions_probs[action] = new_prob;
        }

        for (int action = 0; action < n_game_actions_; action++)
        {
            out_actions_probs[action] /= sum_probs;
        }
    }

    float UctNode::rollout(common::State *state_ptr)
    {
        int depth = 0;
        common::State *rollout_state_ptr = state_ptr;
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
            if (depth > 0)
            {
                delete rollout_state_ptr;
            }
            rollout_state_ptr = new_state;
            depth++;
        }
        common::GameResult result = rollout_state_ptr->getResult();
        delete rollout_state_ptr;
        float z;

        if (result == common::GameResult::win)
        {
            z = 1.0f;
        }
        else if (result == common::GameResult::draw)
        {
            z = 0.0f;
        }
        else // common::GameResult::loss
        {
            z = -1.0f;
        }

        if (depth % 2 != 0)
        {
            z = -z;
        }
        return z;
    }

    UctNode::~UctNode()
    {
        if (children_.size() != 0)
        {
            for (int child_idx = 0; child_idx < children_.size(); child_idx++)
            {
                if (children_[child_idx] != nullptr)
                {
                    if (children_[child_idx]->state_ptr_ != nullptr)
                    {
                        delete children_[child_idx]->state_ptr_;
                    }
                    delete children_[child_idx];
                }
            }
        }
        children_.clear();
        actions_legality_.clear();
        nsa_.clear();
        qsa_.clear();

        children_.shrink_to_fit();
        actions_legality_.shrink_to_fit();
        nsa_.shrink_to_fit();
        qsa_.shrink_to_fit();
    }
} // namespace searchTrees
