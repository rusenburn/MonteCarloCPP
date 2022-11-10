#ifndef SEARCH_TREES_MCRAVE_NODE_HPP_
#define SEARCH_TREES_MCRAVE_NODE_HPP_

#include <vector>
#include <chrono>
#include "../../common/state.hpp"
#include "../../common/terminal_status.hpp"
namespace searchTrees
{
    class McraveNode
    {
    private:
        common::State *state_ptr_;
        int n_game_actions_;
        common::TerminalStatus terminal_;
        bool is_leaf_node;
        bool is_game_result_cached_;
        common::GameResult game_result_;
        std::vector<McraveNode *> children_;
        std::vector<int> nsa_;
        std::vector<float> qsa_;

        std::vector<int> n_sa_;
        std::vector<float> q_sa_;

        std::vector<int> actions_legality_;

        float simulateOne(float b, std::vector<int> &out_our_actions, std::vector<int> &out_their_actions);
        void getFinalProbabilities(std::vector<float> &out_actions_probs);
        int selectMove(float b);
        virtual void heuristic();
        float simDefault(std::vector<int> &out_our_actions, std::vector<int> &out_their_actions);

    public:
        McraveNode(common::State* state_ptr,int n_game_actions);

        ~McraveNode();

        void search(int minimum_simulations_count, std::chrono::duration<int, std::milli> minimum_duration, float b,std::vector<float> &out_actions_probs);
    };
} // namespace searchTrees

#endif