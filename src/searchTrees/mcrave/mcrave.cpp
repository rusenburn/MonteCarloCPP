#include <vector>

#include "mcrave.hpp"

namespace searchTrees
{
    Mcrave::Mcrave(int n_game_actions,float b)
        : n_game_actions_(n_game_actions),
        b_(b) {}

    std::vector<float> Mcrave::search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature)
    {
        McraveNode node(state_ptr, n_game_actions_);
        std::vector<float> actions_probs(n_game_actions_);
        node.search(simulation_count, duration, b_,actions_probs);
        return actions_probs;
    }
} // namespace searchTrees
