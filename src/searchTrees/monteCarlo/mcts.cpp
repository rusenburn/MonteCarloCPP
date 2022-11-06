
#include "mcts.hpp"

namespace searchTrees
{
    MCTS::MCTS(int n_game_actions, common::Evaluator *evaluator_ptr, float cpuct)
        : n_game_actions_(n_game_actions),
          evaluator_ptr_(evaluator_ptr),
          cpuct_(cpuct),
          root_node_ptr_(nullptr)
    {
    }

    std::vector<float> MCTS::search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature)
    {
        root_node_ptr_ = std::make_unique<Node>(state_ptr, n_game_actions_, cpuct_);
        std::vector<float> actions_probs = root_node_ptr_->search(evaluator_ptr_, simulation_count, duration, temperature);
        return actions_probs;
    }
} // namespace searchTrees
