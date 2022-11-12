#ifndef SEARCH_TREES_GRAVE_HPP_
#define SEARCH_TREES_GRAVE_HPP_

#include "../../common/search_tree.hpp"
namespace searchTrees
{
    class Grave : public common::SearchTree
    {
        int n_game_actions_;
        int min_ref_count_;
        float b_squared_;
        bool save_illegal_amaf_actions;

    public:
        Grave(int n_game_actions,int min_ref_count,float b_squared_,bool save_illegal_amaf_actions);
        std::vector<float> search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> minimum_duration, float temperature) override;
    };
} // namespace searchTrees

#endif
