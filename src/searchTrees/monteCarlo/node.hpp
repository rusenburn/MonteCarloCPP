#ifndef SEARCH_TREES_MONTE_CARLO_NODE_HPP_
#define SEARCH_TREES_MONTE_CARLO_NODE_HPP_

#include <vector>
#include <chrono>
#include "../../common/state.hpp"
#include "../../common/evaluator.hpp"

#include "../../common/terminal_status.hpp"

namespace searchTrees
{
    class Node
    {
    private:
        common::State *state_ptr_;
        std::vector<int> actions_legality_;
        float cpuct_;
        int n_game_actions_;
        std::vector<Node *> children_;
        std::vector<float> actions_probs_;
        int n_;

        // number of times action "a" was visited
        std::vector<int> na_;

        // number of times action "a" won
        std::vector<float> wa_;

        // number of times action "a" drew
        std::vector<float> da_;

        // number of times action "a" lost
        std::vector<float> la_;

        common::TerminalStatus terminal_;

        std::vector<float> game_result_;

        std::vector<float> simulateOne(common::Evaluator *evaluator_ptr);
        std::vector<float> getFinalProbabilities(float temperature);
        int getBestAction();

    public:
        Node(common::State *state_ptr, int n_game_actions, float cpuct);
        std::vector<float> search(common::Evaluator *evaluator_ptr, int minimum_simulations_count, std::chrono::duration<int, std::milli> minimum_duration, float temperature);
        ~Node();
    };

} // namespace searchTrees

#endif