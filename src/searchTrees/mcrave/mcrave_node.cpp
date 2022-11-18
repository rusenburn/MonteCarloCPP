#include "mcrave_node.hpp"

namespace searchTrees
{

    McraveNode::McraveNode(common::State *state_ptr, int n_game_actions)
        : state_ptr_(state_ptr), n_game_actions_(n_game_actions), terminal_(common::TerminalStatus::Unknown),
          is_leaf_node(true),
          is_game_result_cached_(false),
          game_result_(common::GameResult::loss),
          children_(std::vector<McraveNode *>(n_game_actions, nullptr)),
          n_sa_(std::vector<int>(n_game_actions)),
          nsa_(std::vector<int>(n_game_actions)),
          q_sa_(std::vector<float>(n_game_actions)),
          qsa_(std::vector<float>(n_game_actions)),
          actions_legality_(std::vector<int>())
    {
        heuristic();
    }
    void McraveNode::search(int minimum_simulations_count, std::chrono::duration<int, std::milli> minimum_duration, float b, std::vector<float> &out_actions_probs)
    {

        if (terminal_ == common::TerminalStatus::Unknown)
        {
            terminal_ = state_ptr_->isTerminal() ? common::TerminalStatus::Done : common::TerminalStatus::NotYet;
        }
        if (terminal_ == common::TerminalStatus::Done)
        {
            throw "State is done";
        }
        auto t_start =std::chrono::high_resolution_clock::now();
        auto t_end = t_start + minimum_duration;
        std::vector<int> out_our_actions{};
        std::vector<int> out_their_actions{};
        float z;
        int i=0;
        for (; i < minimum_simulations_count; i++)
        {
            z = simulateOne(0, out_their_actions, out_our_actions);
            out_our_actions.clear();
            out_their_actions.clear();
        }

        while (t_end > std::chrono::high_resolution_clock::now())
        {
            z = simulateOne(0, out_their_actions, out_our_actions);
            out_our_actions.clear();
            out_their_actions.clear();
            i++;
        }

        int action = selectMove(b);
        out_actions_probs.resize(n_game_actions_);
        out_actions_probs[action] = 1;
    }

    float McraveNode::simulateOne(float b, std::vector<int> &out_our_actions, std::vector<int> &out_their_actions)
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
                is_game_result_cached_ = true;
            }

            if (game_result_ == common::GameResult::win)
            {
                return -1.0f;
            }
            else if (game_result_ == common::GameResult::draw)
            {
                return 0.0f;
            }
            else
            {
                return 1.0f;
            }
        }

        if (is_leaf_node) // leaf node -> first visit
        {
            if (actions_legality_.size() == 0)
            {
                actions_legality_ = state_ptr_->getActionsLegality();
            }
            float z = simDefault(out_our_actions, out_their_actions);
            is_leaf_node = false;
            for (int &action : out_our_actions)
            {
                if (actions_legality_[action] == 0)
                    continue;
                n_sa_[action]++;
                q_sa_[action] += (z - q_sa_[action]) / n_sa_[action];
            }
            return -z;
        }

        int best_action = selectMove(b);

        if (children_[best_action] == nullptr)
        {
            auto new_state_ptr = state_ptr_->step(best_action);
            McraveNode *new_node_ptr = new McraveNode(new_state_ptr, n_game_actions_);
            children_[best_action] = new_node_ptr;
        }
        McraveNode *new_node_ptr = children_[best_action];

        float z = new_node_ptr->simulateOne(b, out_their_actions, out_our_actions);

        for (int &action : out_our_actions)
        {
            if (actions_legality_[action] == 0)
                continue;
            n_sa_[action]++;
            q_sa_[action] += (z - q_sa_[action]) / n_sa_[action];
        }
        out_our_actions.push_back(best_action);

        nsa_[best_action]++;
        qsa_[best_action] += (z - qsa_[best_action]) / nsa_[best_action];
        return -z;
    }

    int McraveNode::selectMove(float b)
    {
        float max_eval = -std::numeric_limits<float>::infinity();
        int best_action = -1;
        for (int action = 0; action < n_game_actions_; action++)
        {
            if (actions_legality_[action] == 0)
                continue;

            float beta = n_sa_[action] / float(nsa_[action] + n_sa_[action] + 4 * b * b * nsa_[action] * n_sa_[action] + float(1e-8));

            float eval = (1 - beta) * qsa_[action] + beta * q_sa_[action];
            if (eval > max_eval)
            {
                max_eval = eval;
                best_action = action;
            }
        }
        if (best_action == -1)
        {
            throw "best action is -1";
        }
        return best_action;
    }

    float McraveNode::simDefault(std::vector<int> &out_our_actions, std::vector<int> &out_their_actions)
    {
        if (terminal_ != common::TerminalStatus::NotYet)
        {
            throw "terminal status was not calculated or done";
        }

        // pick legal action
        int iter = 0;
        common::State *rollout_state_ptr = state_ptr_;
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
            if (iter % 2 == 0)
            {
                out_our_actions.push_back(random_action);
            }
            else
            {
                out_their_actions.push_back(random_action);
            }
            if (iter > 0)
            {
                // dont delete first state
                delete rollout_state_ptr;
            }
            rollout_state_ptr = new_state;
            iter++;
        }
        common::GameResult result = rollout_state_ptr->getResult();

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

        if (iter % 2 != 0)
        {
            z = -z;
        }
        if (iter > 0) // should always be bigger than 0 ; but this is for safety
        {
            delete rollout_state_ptr;
        }

        // relative to the caller 0
        return z;
    }
    void McraveNode::heuristic()
    {
        if (terminal_ == common::TerminalStatus::Unknown)
        {
            terminal_ = state_ptr_->isTerminal() ? common::TerminalStatus::Done : common::TerminalStatus::NotYet;
        }
        if (terminal_ == common::TerminalStatus::Done)
        {
            return;
        }
        if (actions_legality_.size() == 0)
        {
            actions_legality_ = state_ptr_->getActionsLegality();
        }
        for (int action = 0; action < n_game_actions_; action++)
        {
            if (actions_legality_[action] == 0)
                continue;

            // TODO: change these
            qsa_[action] = 0;
            n_sa_[action] = 0;
            q_sa_[action] = 0;
            n_sa_[action] = 50;
        }
    }
    McraveNode::~McraveNode()
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
    }
} // namespace searchTrees
