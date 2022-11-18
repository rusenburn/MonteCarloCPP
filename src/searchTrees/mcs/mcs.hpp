#ifndef SEARCH_TREES_MCS_HPP_
#define SEARCH_TREES_MCS_HPP_

#include "../../common/search_tree.hpp"
#include <vector>
namespace searchTrees
{
    // MonteCarlo Search , not an actual tree
    class MCS : public common::SearchTree
    {
    private:
        int n_game_actions_;
        float cuct_;

        class Node
        {
        private:
            common::State *state_ptr_;
            int n_game_actions_;
            float cuct_;
            float temperature_;
            std::vector<int> actions_legality_;
            // number of simulation ran inside the node
            int n_;
            // N(s,a) , number of visits for each action "a"
            std::vector<int> na_;
            // number of wins for each action "a"
            std::vector<float> nwa_;
            // number of draws for each action "a"
            std::vector<float> nda_;
            // losses count for each action "a"
            std::vector<float> nla_;
            void simulateOne();
            void getFinalProbabilities(std::vector<float> &out_action_probs);
            int getBestAction();
            void rollout(int action, std::vector<float> &out_wdl);

        public:
            Node(common::State *state_ptr, int n_game_actions, float cuct, float temperature);
            void search(int minimum_simulation_count, std::chrono::duration<int, std::milli> minimum_duration, std::vector<float> &out_actions_probs);
        };

    public:
        MCS(int n_game_actions, float cuct);
        std::vector<float> search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature) override;
    };

} // namespace name

#endif