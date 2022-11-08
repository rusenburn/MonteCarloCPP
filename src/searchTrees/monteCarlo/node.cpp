#include <memory>
#include <vector>
#include <math.h>
#include <numeric>
#include <algorithm>
#include "node.hpp"
#include "../../common/game_result.hpp"

namespace searchTrees
{
    Node::Node(common::State *state_ptr, int n_game_actions, float cpuct)
        : state_ptr_(state_ptr),
          n_game_actions_(n_game_actions),
          cpuct_(cpuct),
          actions_legality_(std::vector<int>()), // no size means first visit
          children_(std::vector<Node *>(n_game_actions, nullptr)),
          actions_probs_(std::vector<float>()),
          n_(0),
          na_(std::vector<int>(n_game_actions, 0)),
          wa_(std::vector<float>(n_game_actions, 0.0f)),
          da_(std::vector<float>(n_game_actions, 0.0f)),
          la_(std::vector<float>(n_game_actions, 0.0f)),
          terminal_(TerminalState::Unknown),
          game_result_(std::vector<float>())
    {
    }

    std::vector<float> Node::search(common::Evaluator *evaluator_ptr, int minimum_simulations_count, std::chrono::duration<int, std::milli> minimum_duration, float temperature)
    {
        if (terminal_ == TerminalState::Unknown)
        {
            terminal_ = state_ptr_->isTerminal() ? TerminalState::Done : TerminalState::NotYet;
        }
        if (terminal_ == TerminalState::Done)
        {
            throw "Game is over";
        }

        auto t_end = std::chrono::high_resolution_clock::now() + minimum_duration;

        for (int i = 0; i < minimum_simulations_count; i++)
        {
            simulateOne(evaluator_ptr);
        }

        while (t_end > std::chrono::high_resolution_clock::now())
        {
            simulateOne(evaluator_ptr);
        }
        std::vector<float> actions_probs = getFinalProbabilities(temperature);
        return actions_probs;
    }

    std::vector<float> Node::simulateOne(common::Evaluator *evaluator_ptr)
    {
        if (terminal_ == TerminalState::Unknown) // first visit check terminal state
        {
            terminal_ = state_ptr_->isTerminal() ? TerminalState::Done : TerminalState::NotYet;
        }
        if (terminal_ == TerminalState::Done) // cache terminal state return its reverse
        {
            if (game_result_.size() == 0) // game result is not cached yet , cache it
            {
                auto res = state_ptr_->getResult();
                if (res == common::GameResult::win)
                {
                    game_result_ = {1.0f, 0.0f, 0.0f};
                }
                else if (res == common::GameResult::draw)
                {
                    game_result_ = {0.0f, 1.0f, 0.0f};
                }
                else
                {
                    game_result_ = {0.0f, 0.0f, 1.0f};
                }
            }

            std::vector<float> wdl = {0.0f, 0.0f, 0.0f};
            wdl[0] = game_result_[2];
            wdl[1] = game_result_[1];
            wdl[2] = game_result_[0];
            return wdl;
        }

        if (actions_legality_.size() == 0) // if actions_legality_ size is 0 then this is the first visit
        {
            actions_legality_ = state_ptr_->getActionsLegality();
            common::Evaluation eval = evaluator_ptr->evaluate(state_ptr_);
            actions_probs_ = eval.action_probs;

            // reverse it

            float temp = eval.wdl[2];
            eval.wdl[2] = eval.wdl[0];
            eval.wdl[0] = temp;
            return eval.wdl;
        }

        int best_action = getBestAction();

        if (children_[best_action] == nullptr) // child node not initialized yet , initialize it
        {
            auto new_state_ptr = state_ptr_->step(best_action);
            searchTrees::Node *new_node_ptr = new Node(new_state_ptr, n_game_actions_, cpuct_);
            children_[best_action] = new_node_ptr;
        }

        searchTrees::Node *new_node_ptr = children_[best_action];

        std::vector<float> wdl = new_node_ptr->simulateOne(evaluator_ptr);

        wa_[best_action] += wdl[0];
        da_[best_action] += wdl[1];
        la_[best_action] += wdl[2];

        na_[best_action] += 1;
        n_++;

        float temp = wdl[2];
        wdl[2] = wdl[0];
        wdl[0] = temp;
        return wdl;
    }

    std::vector<float> Node::getFinalProbabilities(float temperature)
    {
        if (temperature == 0)
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

            std::vector<float> result{};
            result.reserve(n_game_actions_);
            for (int action = 0; action < n_game_actions_; action++)
            {
                result.emplace_back(action == best_action ? 1 : 0);
            }
            return result;
        }

        std::vector<float> probs_with_temperature{};
        probs_with_temperature.reserve(n_game_actions_);
        float sum_probs = 0;
        for (int action = 0; action < n_game_actions_; action++)
        {
            // should be na_[action]/n;
            float new_prob = pow(float(na_[action]), 1.0f / temperature);
            sum_probs += new_prob;
            probs_with_temperature.emplace_back(new_prob);
        }

        std::vector<float> result{};
        result.reserve(n_game_actions_);
        for (int action = 0; action < n_game_actions_; action++)
        {
            result.emplace_back(probs_with_temperature[action] / sum_probs);
        }
        return result;
    }

    int Node::getBestAction()
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
                // action is not legal skip current iteration
                continue;
            }

            float na = float(na_[action]);

            // Q(s,a)
            float qsa = 0;
            if (na > 0)
            {
                float wa = wa_[action];
                float la = la_[action];
                qsa = (wa - la) / na;
            }
            float u = qsa + cpuct_ * actions_probs_[action] * sqrtf(n_ + 1e-8f) / (1 + na);

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

    Node::~Node()
    {
        // Note : Node does not own its  state_ptr and should not delete it
        //      however this Node owns all its children state_ptrs, therefore should delete them

        if (children_.size() != 0)
        {
            // Delete every child node pointer and its state pointer if it exist

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
            children_.clear();
            game_result_.clear();
            actions_probs_.clear();
            na_.clear();
            wa_.clear();
            da_.clear();
            la_.clear();
            actions_legality_.clear();

            children_.shrink_to_fit();
            actions_probs_.shrink_to_fit();
            game_result_.shrink_to_fit();
            na_.shrink_to_fit();
            wa_.shrink_to_fit();
            da_.shrink_to_fit();
            la_.shrink_to_fit();
            actions_legality_.shrink_to_fit();
        }
    }
} // namespace searchTrees
