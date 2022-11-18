#include <math.h>

#include "grave_node.hpp"
namespace searchTrees
{
    GraveNode::GraveNode(common::State *state_ptr, int n_game_actions)
        : state_ptr_(state_ptr),
          n_game_actions_(n_game_actions),
          terminal_(common::TerminalStatus::Unknown),
          is_leaf_node_(true),
          is_game_result_cached_(false),
          children_(std::vector<GraveNode *>(n_game_actions)),
          n_(0),
          wsa_(std::vector<float>(n_game_actions)),
          nsa_(std::vector<int>(n_game_actions)),
          amaf_wsa_player_0(std::vector<float>(n_game_actions)),
          amaf_nsa_player_0(std::vector<int>(n_game_actions)),
          amaf_wsa_player_1(std::vector<float>(n_game_actions)),
          amaf_nsa_player_1(std::vector<int>(n_game_actions)),
          actions_legality_(std::vector<int>())
    {
        heuristic();
    }
    float GraveNode::simulateOne(GraveNode *amaf_node_ptr,
                                 bool save_illegal_amaf_actions,
                                 int depth,
                                 std::vector<int> &out_our_actions,
                                 std::vector<int> &out_opponent_actions,
                                 const int &amaf_min_ref_count,
                                 const float &b_square_ref)
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
        if (is_leaf_node_) // leaf node first non terminal visit
        {
            if (actions_legality_.size() == 0)
            {
                actions_legality_ = state_ptr_->getActionsLegality();
            }
            float z = playout(out_our_actions, out_opponent_actions);
            is_leaf_node_ = false;

            update_amaf(z, out_our_actions, out_opponent_actions, depth, save_illegal_amaf_actions);
            // for (int &action : out_our_actions)
            // {
            //     if (!save_illegal_amaf_actions && actions_legality_[action] == 0)
            //         continue;
            //     if (depth % 2 == 0)
            //     {
            //         amaf_nsa_player_0[action]++;
            //         amaf_wsa_player_0[action] += z;
            //     }
            //     else
            //     {
            //         amaf_nsa_player_1[action]++;
            //         amaf_wsa_player_1[action] += z;
            //     }
            // }

            // for (int &action : out_opponent_actions)
            // {
            //     if (!save_illegal_amaf_actions && actions_legality_[action] == 0)
            //         continue;
            //     if (depth % 2 == 0)
            //     {
            //         amaf_nsa_player_1[action]++;
            //         amaf_wsa_player_1[action] -= z;
            //     }
            //     else
            //     {
            //         amaf_nsa_player_0[action]++;
            //         amaf_wsa_player_0[action] -= z;
            //     }
            // }
            return -z;
        }
        if (n_ >= amaf_min_ref_count)
        {
            amaf_node_ptr = this;
        }
        int best_action = selectMove(amaf_node_ptr, depth, b_square_ref);

        GraveNode *new_node_ptr = children_[best_action];
        if (new_node_ptr == nullptr)
        {
            common::State *new_state_ptr = state_ptr_->step(best_action);
            new_node_ptr = new GraveNode(new_state_ptr, n_game_actions_);
            children_[best_action] = new_node_ptr;
        }

        float z = new_node_ptr->simulateOne(amaf_node_ptr, save_illegal_amaf_actions, depth + 1, out_opponent_actions, out_our_actions, amaf_min_ref_count, b_square_ref);

        update_amaf(z,out_our_actions,out_opponent_actions,depth,save_illegal_amaf_actions);

        out_our_actions.push_back(best_action);
        n_++;
        nsa_[best_action]++;
        wsa_[best_action] += z;
        return -z;
    }

    int GraveNode::selectMove(GraveNode *amaf_node_ptr, int depth, const float &b_square_ref)
    {
        int best_action = -1;
        float best_value = -std::numeric_limits<float>::infinity();
        for (int action = 0; action < n_game_actions_; action++)
        {
            if (actions_legality_[action] == 0)
            {
                continue;
            }
            float w = wsa_[action];
            float p = float(nsa_[action]);
            float wa = 0.0f;
            float pa = 1e-8f;
            if (amaf_node_ptr != nullptr)
            {
                wa = depth % 2 == 0 ? amaf_wsa_player_0[action] : amaf_wsa_player_1[action];
                pa = float(depth % 2 == 0 ? amaf_nsa_player_0[action] : amaf_nsa_player_1[action]);
            }
            float beta_action = pa / (pa + p + b_square_ref * pa * p);
            float amaf = wa / (pa + 1e-8f);
            float mean = w / (p + 1e-8f);
            float value = (1.0f - beta_action) * mean + beta_action * amaf;
            if (value > best_value)
            {
                best_value = value;
                best_action = action;
            }
        }
        return best_action;
    }

    float GraveNode::playout(std::vector<int> &out_our_actions, std::vector<int> &out_opponent_actions)
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
            for (int action = 0; action < n_game_actions_; action++)
            {
                n_legal_actions += rollout_actions_legality[action];
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
                out_opponent_actions.push_back(random_action);
            }
            if (iter > 0)
            {
                // don't delete first state
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
    void GraveNode::heuristic()
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
            // if (actions_legality_[action] == 0)
            //         continue;

            wsa_[action] = 0;
            nsa_[action] = 0;
            amaf_wsa_player_0[action] = 0;
            amaf_nsa_player_0[action] = 50;
            amaf_wsa_player_1[action] = 0;
            amaf_nsa_player_1[action] = 50;
        }
    }

    void GraveNode::update_amaf(float our_score,const std::vector<int> &our_actions_ref,const std::vector<int> &opponent_actions_ref, int depth, bool save_illegal_amaf_actions)
    {
        for (int action : our_actions_ref)
        {
            if (!save_illegal_amaf_actions && actions_legality_[action] == 0)
                continue;
            if (depth % 2 == 0)
            {
                amaf_nsa_player_0[action]++;
                amaf_wsa_player_0[action] += our_score;
            }
            else
            {
                amaf_nsa_player_1[action]++;
                amaf_wsa_player_1[action] += our_score;
            }
        }

        for (int action : opponent_actions_ref)
        {
            if (!save_illegal_amaf_actions && actions_legality_[action] == 0)
                continue;
            if (depth % 2 == 0)
            {
                amaf_nsa_player_1[action]++;
                amaf_wsa_player_1[action] -= our_score;
            }
            else
            {
                amaf_nsa_player_0[action]++;
                amaf_wsa_player_0[action] -= our_score;
            }
        }
    }
    GraveNode::~GraveNode()
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
} // namespace searctTrees
