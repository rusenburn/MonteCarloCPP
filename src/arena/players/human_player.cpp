#include <iostream>
#include <sstream>
#include "human_player.hpp"
namespace arena
{
    int HumanPlayer::chooseAction(common::State *state_ptr)
    {
        state_ptr->render();
        std::cout << '\n'
                  << "Choose Action : ";
        std::string in;
        int action;
        std::getline(std::cin, in);
        std::stringstream(in) >> action;
        return action;
    }
} // namespace arena
