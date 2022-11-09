#ifndef SEARCH_TREES_UCT_NODE_HPP_
#define SEARCH_TREES_UCT_NODE_HPP_

#include <chrono>
#include <array>
#include "../../common/state.hpp"
#include "../../common/terminal_status.hpp"


namespace searchTrees
{
    class UctNode
    {
    private:
        common::State *state_ptr_;
        int n_game_actions_;
        float cuct_;

        common::TerminalStatus terminal_;
        bool is_game_result_cached_;
        common::GameResult game_result_;
        std::vector<int> actions_legality_;
        std::vector<UctNode *> children_;

        int n_;
        // Q(s,a)
        std::vector<float> qsa_;

        // N(n,a)
        std::vector<int> nsa_;

        float simulateOne();
        int findBestAction();
        void getFinalProbabilities(float temperature, std::vector<float> &out_actions_probs);
        float rollout(common::State *state_ptr);

    public:
        UctNode(common::State *state_ptr, int n_game_actions, float cuct);
        void search(int minimum_simulations, std::chrono::duration<int, std::milli> minimum_duration, float uct, float temperature, std::vector<float> &out_actions_probs);
        ~UctNode();
    };
} // namespace searchTrees

#endif