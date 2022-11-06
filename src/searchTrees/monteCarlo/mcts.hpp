#ifndef SEARCH_TREES_MCTS_HPP_
#define SEARCH_TREES_MCTS_HPP_

#include <memory>
#include "../../common/search_tree.hpp"
#include "../../common/evaluator.hpp"
#include "../../common/state.hpp"

#include "node.hpp"

namespace searchTrees
{
    class MCTS : public common::SearchTree
    {
    private:
        int n_game_actions_;
        common::Evaluator *evaluator_ptr_;
        std::unique_ptr<Node> root_node_ptr_;
        float cpuct_;

    public:
        MCTS(int n_game_actions, common::Evaluator *evaluator_ptr, float cpuct);
        std::vector<float> search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature) override;
    };
} // namespace searchTrees

#endif
