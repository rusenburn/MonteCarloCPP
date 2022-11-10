#ifndef SEARCH_TREES_MCRAVE_HPP_
#define SEARCH_TREES_MCRAVE_HPP_

#include "mcrave_node.hpp"
#include "../../common/search_tree.hpp"
#include "../../common/state.hpp"
namespace searchTrees
{
    class Mcrave : public common::SearchTree
    {
    private:
        int n_game_actions_;
        float b_;

    public:
        Mcrave(int n_game_actions, float b);
        std::vector<float> search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature) override;
    };
} // namespace searchTrees

#endif