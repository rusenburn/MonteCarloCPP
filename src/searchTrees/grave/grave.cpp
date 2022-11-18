#include <iostream>
#include <chrono>
#include "grave.hpp"
#include "grave_node.hpp"
namespace searchTrees
{
    Grave::Grave(int n_game_actions, int min_ref_count, float b_squared, bool save_illegal_amaf_actions)
        : n_game_actions_(n_game_actions),
          min_ref_count_(min_ref_count),
          b_squared_(b_squared),
          save_illegal_amaf_actions(save_illegal_amaf_actions){};

    std::vector<float> Grave::search(common::State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> minimum_duration, float temperature)
    {
        if (state_ptr->isTerminal())
        {
            throw "State is done";
        }
        GraveNode node(state_ptr, n_game_actions_);
        auto t_end = std::chrono::high_resolution_clock::now() + minimum_duration;

        std::vector<int> out_our_actions{};
        std::vector<int> out_their_actions{};
        float z;
        int i = 0;
        for (; i < simulation_count; i++)
        {
            z = node.simulateOne(&node, save_illegal_amaf_actions, 0, out_our_actions, out_their_actions, min_ref_count_, b_squared_);
            out_our_actions.clear();
            out_their_actions.clear();
        }

        while (t_end > std::chrono::high_resolution_clock::now())
        {
            z = node.simulateOne(&node, save_illegal_amaf_actions, 0, out_their_actions, out_our_actions, min_ref_count_, b_squared_);
            out_our_actions.clear();
            out_their_actions.clear();
            i++;
        }

        int action = node.selectMove(&node, 0,b_squared_);
        std::vector<float> action_probs(n_game_actions_);
        action_probs[action] = 1;
        // std::cout<< i << std::endl;
        return action_probs;
    }
} // namespace searchTrees
