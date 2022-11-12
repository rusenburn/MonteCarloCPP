#include <iostream>
#include <vector>
#include "match.hpp"
#include "../common/game_result.hpp"
namespace arena
{
    Match::Match(std::shared_ptr<common::Game> (*game_fn_ptr)(), common::Player *player_1_ptr, common::Player *player_2_ptr, int n_sets, bool render)
        : game_ptr_(game_fn_ptr()),
          player1_ptr_(player_1_ptr),
          player2_ptr_(player_2_ptr),
          n_sets_(n_sets),
          render_(render),
          scores_(std::vector<int>(3, 0)) {}

    std::vector<int> Match::start()
    {
        int starting_player = 0;
        for (int set_index = 0; set_index < n_sets_; set_index++)
        {
            std::vector<int> match_scores = play_set(starting_player);
            scores_[0] += match_scores[0]; // player 1 wins
            scores_[1] += match_scores[1]; // draws
            scores_[2] += match_scores[2]; // player 2 wins

            // switch starting player
            starting_player = 1 - starting_player;
            std::cout << "[ " << scores_[0] << " ," << scores_[1] << " ," << scores_[2] << " ]" << std::endl;
        }
        return scores_;
    }

    std::vector<int> Match::play_set(int starting_player)
    {
        std::vector<common::Player *> players_ptrs = {player1_ptr_, player2_ptr_};
        common::State *state_ptr = game_ptr_->reset();
        common::State *new_state_ptr(nullptr);
        bool isDone(false);
        int current_player = starting_player;
        common::Player *player_ptr(nullptr);

        std::vector<int> result = {0, 0, 0};

        while (!state_ptr->isTerminal())
        {
            if (render_)
            {
                state_ptr->render();
            }
            player_ptr = players_ptrs[current_player];

            int action = player_ptr->chooseAction(state_ptr);

            std::vector<int> actions_legality = state_ptr->getActionsLegality();

            if (actions_legality[action] == 0)
            {
                std::cout << "player " << current_player + 1 << " chose wrong action of " << action << '\n';
                continue;
            }

            new_state_ptr = game_ptr_->step(action);
            delete state_ptr;
            state_ptr = new_state_ptr;
            current_player = 1 - current_player;
        }

        common::GameResult r = state_ptr->getResult();
        if (r == common::GameResult::win)
        {
            result[0] = 1;
        }
        else if (r == common::GameResult::draw)
        {
            result[1] = 1;
        }
        else
        {
            result[2] = 1;
        }

        if (current_player != 0)
        {
            // reverse the result
            int temp = result[0];
            result[0] = result[2];
            result[2] = temp;
        }
        if (render_)
        {
            state_ptr->render();
        }
        delete state_ptr; // state_ptr and new_state_ptr are the same
        return result;
    }
} // namespace arena
