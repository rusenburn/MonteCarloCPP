#include <iostream>
#include <iomanip>
#include "connect4_state.hpp"

namespace games
{
    Connect4State::Connect4State(std::array<std::array<int, N_COLS>, N_ROWS> observation, int last_action, int turn)
        : observation_(observation),
          last_action_(last_action),
          turn_(turn),
          actions_legality_(std::vector<int>()),
          is_game_result_cached_(false),
          game_result_(common::GameResult::draw),
          is_terminal_cached_(false),
          is_terminal_(false)
    {
    }

    std::vector<int> Connect4State::getActionsLegality()
    {
        if (actions_legality_.size() > 0)
            return actions_legality_;

        actions_legality_ = std::vector<int>(N_COLS, 0);
        for (int action = 0; action < N_COLS; action++)
        {
            if (isLegalAction(action))
            {
                actions_legality_[action] = 1;
            }
        }

        return actions_legality_;
    }

    bool Connect4State::isTerminal()
    {
        if (last_action_ < 0)
            return false;
        if (is_terminal_cached_)
        {
            return is_terminal_;
        }
        int other_player = 1;
        if (isWinning(other_player) || isFull())
        {
            is_terminal_ = true;
        }
        else
        {
            is_terminal_ = false;
        }
        is_terminal_cached_ = true;

        return is_terminal_;
    }

    common::GameResult Connect4State::getResult()
    {
        if (!isTerminal())
        {
            throw "Game is not terminal";
        }
        int player = 0;
        int other = 1;

        if (isWinning(other))
        {
            game_result_ = common::GameResult::loss;
        }
        else if (isFull())
        {
            game_result_ = common::GameResult::draw;
        }
        else
        {
            // should not be here
            throw "Impossible for a player to move to be already won";
        }
        is_game_result_cached_ = true;
        return game_result_;
    }

    Connect4State *Connect4State::step(int action)
    {
        std::vector<int> actions_legality = getActionsLegality();
        if (actions_legality[action] == 0)
        {
            throw "Invalid action";
        }

        std::array<std::array<int, N_COLS>, N_ROWS> opponent_observation;
        for (int row = 0; row < N_ROWS; row++)
        {
            for (int col = 0; col < N_COLS; col++)
            {
                opponent_observation[row][col] = -observation_[row][col];
            }
        }
        int action_row = N_ROWS;
        for (int row = 0; row < N_ROWS; row++)
        {
            if (opponent_observation[row][action] == 0)
            {
                action_row = row;
                break;
            }
        }
        if (action_row == N_ROWS)
        {
            // Should not be here
            throw "Out of range";
        }

        // from out opponent perspective we are the one that moved , so -1;
        opponent_observation[action_row][action] = -1;
        return new Connect4State(opponent_observation, action, turn_ + 1);
    }

    void Connect4State::render()
    {
        printf("****************************\n");
        printf("*** Player X has to move ***\n");
        printf("****************************\n");

        for (int row = 0; row < N_ROWS; row++)
        {
            printf("\n");
            printf("____________________________\n");
            for (int col = 0; col < N_COLS; col++)
            {
                printf("|");
                if (observation_[6 - row - 1][col] == 1)
                {
                    printf(" X ");
                }
                else if (observation_[6 - row - 1][col] == -1)
                {
                    printf(" O ");
                }
                else
                {
                    printf("   ");
                }
                if (col == N_COLS - 1)
                {
                    printf("|");
                }
            }

            if (row == N_ROWS - 1)
            {
                printf("\n----------------------------");
            }
        }
        printf("\n");
        for (int col = 0; col < N_COLS; col++)
        {
            printf("  %d ", col);
        }
    }

    std::string Connect4State::getShort()
    {
        throw "Not implemented yet";
    }

    std::vector<int> Connect4State::getObservation()
    {
        const int cells_in_channels = N_ROWS * N_COLS;
        std::vector<int> result(N_ROWS * N_COLS * 2, 0);
        result.reserve(N_ROWS * N_COLS * 2);
        int result_channel;
        int result_index;
        for (int row = 0; row < N_ROWS; row++)
        {
            for (int col = 0; col < N_COLS; col++)
            {
                if (observation_[row][col] == 0)
                {
                    continue;
                }
                if (observation_[row][col] == 1)
                {
                    result_channel = 0;
                }
                else // == -1
                {
                    result_channel = 1;
                }
                result_index = result_channel * (cells_in_channels) + N_COLS * row + col;
                result[result_index] = 1;
            }
        }
        return result;
    }

    std::vector<std::tuple<common::State *, std::vector<int>>> Connect4State::getSymmerties()
    {
        throw "Not Implemented yet";
    }

    int Connect4State::getActionsCount()
    {
        return N_COLS;
    }
    common::ObservationSpace Connect4State::getObservationSpace()
    {
        return {2, N_ROWS, N_COLS};
    }
    bool Connect4State::isWinning(int player)
    {
        if (last_action_ < 0)
        {
            return false;
        }

        int row = 0;
        while (row < N_ROWS)
        {
            if (row + 1 == N_ROWS || observation_[row+1][last_action_] == 0)
            {
                break;
            }
            row++;
        }
        if (isVerticalWin(player, row, last_action_))
        {
            return true;
        }

        if (isHorizontalWin(player, row, last_action_))
        {
            return true;
        }

        if (isForwardDiagonalWin(player, row, last_action_))
        {
            return true;
        }

        if (isBackwarDiagonalWin(player, row, last_action_))
        {
            return true;
        }

        return false;
    }

    bool Connect4State::isFull()
    {
        return turn_ >= (N_COLS * N_ROWS - 1);
    }

    bool Connect4State::isVerticalWin(int player, int row, int col)
    {
        int player_cell_value = player == 0 ? 1 : -1;
        int count = 1;
        int next_col = col + 1;
        while (next_col < N_COLS && observation_[row][next_col] == player_cell_value)
        {
            count++;
            next_col++;
        }

        next_col = col - 1;
        while (next_col >= 0 && observation_[row][next_col] == player_cell_value)
        {
            count++;
            next_col--;
        }
        return count >= 4;
    }

    bool Connect4State::isHorizontalWin(int player, int row, int col)
    {
        int player_cell_value = player == 0 ? 1 : -1;
        int count = 1;
        int next_row = row + 1;

        while (next_row < N_ROWS && observation_[next_row][col] == player_cell_value)
        {
            count++;
            next_row++;
        }

        next_row = row - 1;
        while (next_row >= 0 && observation_[next_row][col] == player_cell_value)
        {
            count++;
            next_row--;
        }

        return count >= 4;
    }

    bool Connect4State::isForwardDiagonalWin(int player, int row, int col)
    {
        int player_cell_value = player == 0 ? 1 : -1;
        int count = 1;
        int steps = 1;
        while (row + steps < N_ROWS && col + steps < N_COLS && observation_[row+steps][col+steps] == player_cell_value)
        {
            count++;
            steps++;
        }
        steps = 1;
        while (row - steps >= 0 && col - steps >= 0 && observation_[row - steps][col - steps] == player_cell_value)
        {
            count++;
            steps++;
        }

        return count >= 4;
    }

    bool Connect4State::isBackwarDiagonalWin(int player, int row, int col)
    {
        int player_cell_value = player == 0 ? 1 : -1;
        int count = 1;
        int steps = 1;
        while (row + steps < N_ROWS && col - steps >= 0 && observation_[row + steps][col - steps] == player_cell_value)
        {
            count++;
            steps++;
        }

        steps = 1;
        while (row - steps >= 0 && col + steps < N_COLS && observation_[row - steps][col + steps] == player_cell_value)
        {
            count++;
            steps++;
        }

        return count >= 4;
    }

    bool Connect4State::isLegalAction(int action)
    {
        // last row should be empty for that action(column)
        return observation_[N_ROWS - 1][action] == 0;
    }

} // namespace games
