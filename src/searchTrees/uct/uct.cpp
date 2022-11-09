#include "uct.hpp"
#include "uct_node.hpp"
namespace searchTrees
{
    UctSearchTree::UctSearchTree(int n_game_actions, float cuct)
        : n_game_actions_(n_game_actions),
          cuct_(cuct)
    {
    }

    std::vector<float> UctSearchTree::search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature)
    {
        UctNode node{state_ptr, n_game_actions_, cuct_};
        std::vector<float> actions_probs(n_game_actions_);
        node.search(simulation_count, duration, cuct_, temperature, actions_probs);
        return actions_probs;
    }
} // namespace searchTrees
