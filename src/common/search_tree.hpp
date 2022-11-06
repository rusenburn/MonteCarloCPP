#include <memory>
#include <chrono>
#include <vector>
#include "state.hpp"
namespace common
{
    class SearchTree
    {
    public:
        /// @brief
        /// @param state_ptr pointer for state of the game
        /// @param simulations_count  minimum number of simulations to be done before returning the result
        /// @param duration miminum duration of time for the simulations to be done before returning the result
        /// @param temperature degree of exploration when processing the final probabilities , if temperature == 0 this function will set the highest probability as 1 and others 0
        /// @return array of actions probabilities after performing simulations for some duration
        virtual std::vector<float> search(State *state_ptr, int simulation_count, std::chrono::duration<int, std::milli> duration, float temperature) = 0;

        /// @brief
        /// @param state_ptr pointer for state of the game
        /// @param simulations_count number of simulations to be done before returning the result
        /// @param temperature degree of exploration when processing the final probabilities , if temperature == 0 this function will set the highest probability as 1 and others 0
        /// @return array of actions probabilities after performing a number of simulations
        std::vector<float> search(State *state_ptr, int simulations_count, float temperature)
        {
            return search(state_ptr, simulations_count, std::chrono::duration<int, std::milli>(0), temperature);
        };

        /// @brief
        /// @param state_ptr pointer for state of the game
        /// @param duration duration of time for the simulations to be done before returning the result
        /// @param temperature degree of exploration when processing the final probabilities , if temperature == 0 this function will set the highest probability as 1 and others 0
        /// @return array of actions probabilities after performing simulations for some duration
        std::vector<float> search(State *state_ptr, std::chrono::duration<int, std::milli> duration, float temperature)
        {
            return search(state_ptr, 0, duration, temperature);
        };

        virtual ~SearchTree(){};
    };
} // namespace common
