#ifndef SEARCH_TREES_UCT_HPP_
#define SEARCH_TREES_UCT_HPP_

#include <vector>
#include <chrono>

#include "../../common/search_tree.hpp"
namespace searchTrees
{
    class UctSearchTree : common::SearchTree
    {
    private:
        int n_game_actions_;
        float cuct_;
    public:
        UctSearchTree(int n_game_actions,float cuct);
        std::vector<float> search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature) override;
    };
} // namespace searchTrees

#endif