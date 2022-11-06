#include <iostream>
#include <iomanip>
#include <string>
#include "othello_state.hpp"
namespace games
{
    OthelloState::OthelloState(std::vector<std::vector<std::vector<int8_t>>> observation, int consecutive_skips)
        : observation_(observation),
          n_consecutive_skips_(consecutive_skips),
          actions_legality_()
    {
    }

    std::vector<int> OthelloState::getActionsLegality()
    {
        if (!actions_legality_.empty())
        {
            return actions_legality_;
        }

        int player = observation_[2][0][0];
        int other = 1 - player;
        const int N_ROWS = 8;
        const int N_COLS = 8;

        // TODO PERFORMANCE
        actions_legality_.resize(N_COLS * N_ROWS + 1, 0); // the +1 is the skip action

        bool has_legal_action = false;
        for (int row = 0; row < N_ROWS; row++)
        {
            for (int col = 0; col < N_COLS; col++)
            {
                if (!getBoardChangesOnAction(row, col).empty())
                {
                    actions_legality_[row * N_COLS + col] = 1;
                    has_legal_action = true;
                }
            }
        }

        if (!has_legal_action)
        {
            // if there is no legal actions then a skip action is a valid action
            actions_legality_[N_ROWS * N_COLS] = 1;
        }
        return actions_legality_;
    }

    bool OthelloState::isTerminal()
    {
        if (n_consecutive_skips_ == 2)
            return true;
        const int N_ROWS = 8;
        const int N_COLS = 8;
        for (int row = 0; row < N_ROWS; row++)
        {
            for (int col = 0; col < N_COLS; col++)
            {
                if (!observation_[0][row][col] && !this->observation_[1][row][col])
                    return false;
            }
        }
        return true;
    }

    common::GameResult OthelloState::getResult()
    {
        if (!isTerminal())
        {
            throw "Game is done";
        }

        std::vector<int> wdl = {0, 0, 0};
        int player = observation_[2][0][0];
        int other = 1 - player;

        int player_score = 0;
        int other_score = 0;
        const int N_ROWS = 8;
        const int N_COLS = 8;

        for (int i = 0; i < N_ROWS; i++)
        {
            for (int j = 0; j < N_COLS; j++)
            {
                player_score += this->observation_[player][i][j];
                other_score += this->observation_[other][i][j];
            }
        }

        if (player_score > other_score)
        {
            return common::GameResult::win;
        }
        if (player_score == other_score)
        {
            return common::GameResult::draw;
        }
        return common::GameResult::loss;
    }

    OthelloState *OthelloState::step(int action)
    {
        actions_legality_ = getActionsLegality();
        int action_legality = actions_legality_[action];
        if (action_legality == 0)
        {
            throw "Invalid action";
        }

        int player = this->observation_[2][0][0];
        int other = 1 - player;

        std::vector<std::vector<std::vector<int8_t>>> new_obs(observation_);
        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                new_obs[2][row][col] = other;
            }
        }
        // check if action is a skip action which is 64
        if (action == 64)
        {
            // return new state with an increased number to consecutive skips by 1
            return new OthelloState(new_obs, n_consecutive_skips_ + 1);
        }

        int skips = 0;

        // turn action into (row,col) action
        int row_id = action / 8;
        int col_id = action % 8;

        std::vector<std::pair<int, int>> board_changes = this->getBoardChangesOnAction(row_id, col_id);
        if (board_changes.size() == 0)
        {
            throw "No tiles to flip after picking an action";
        }

        for (auto &cell : board_changes)
        {
            // for each cell
            int cell_row = cell.first;
            int cell_col = cell.second;

            // let our player update his cell value
            new_obs[player][cell_row][cell_col] = 1;

            // let the other player remove the cell value
            new_obs[other][cell_row][cell_col] = 0;
        }

        new_obs[player][row_id][col_id] = 1;
        return new OthelloState(new_obs, skips);
    }

    void OthelloState::render()
    {
        int player_x = 0;
        int player_o = 1 - player_x;
        int obs[8][8];
        // combine current player with other player observations into one
        // by substraction
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int player_x_cell = this->observation_[player_x][i][j];
                int player_o_cell = this->observation_[player_o][i][j];
                obs[i][j] = player_x_cell - player_o_cell;
            }
        }

        auto legal_actions = getActionsLegality();
        // transform legal_actions into 2d form
        int legal_actions_2d[8][8];
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int action = i * 8 + j;
                legal_actions_2d[i][j] = legal_actions[action];
            }
        }

        std::cout << "\n";
        std::cout << "   0 1 2 3 4 5 6 7\n";
        for (int i = 0; i < 8; i++)
        {
            std::cout << std::setw(2) << std::setfill(' ') << i * 8;
            std::cout << ' ';
            for (int j = 0; j < 8; j++)
            {
                char v = '.';
                int current_cell = obs[i][j];
                if (current_cell == 1)
                {
                    v = 'X';
                }
                else if (current_cell == -1)
                {
                    v = 'O';
                }
                if (legal_actions_2d[i][j] == 1)
                {
                    v = ',';
                }
                std::cout << v << ' ';
            }
            std::cout << '\n';
        }
        char v = 'X';
        if (this->observation_[2][0][0] == 1)
        {
            v = 'O';
        }
        std::cout << "\n#Player " << v << " Turn #";
    }

    bool OthelloState::isBoardAction(int row, int col)
    {
        return row < 8 && row >= 0 && col < 8 && col >= 0;
    }

    std::vector<std::pair<int, int>> OthelloState::getBoardChangesOnAction(int row, int col)
    {
        if (!this->isBoardAction(row, col) || this->observation_[0][row][col] != 0 || this->observation_[1][row][col] != 0)
        {
            return std::vector<std::pair<int, int>>();
        }

        int player = this->observation_[2][0][0];
        int other = 1 - player;

        int obs[8][8];

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                int player_cell = this->observation_[player][i][j];
                int other_cell = this->observation_[other][i][j];
                obs[i][j] = player_cell - other_cell;
            }
        }

        obs[row][col] = 1;
        std::vector<std::pair<int, int>> tiles_to_flip;

        const int directions[8][2] = {
            {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

        for (auto &direction : directions)
        {
            int row_direction = direction[0];
            int col_direction = direction[1];
            int x = row;
            int y = col;
            x += row_direction;
            y += col_direction;

            // check if there is an enemy cell next to this cell
            if (isBoardAction(x, y) && obs[x][y] == -1)
            {
                // move to the next cell
                x += row_direction;
                y += col_direction;

                // check if it is not on board
                if (!isBoardAction(x, y))
                {
                    // continue next direction
                    continue;
                }

                // while enemy cell move direction until you leave board or cell color changes
                while (obs[x][y] == -1)
                {
                    x += row_direction;
                    y += col_direction;
                    if (!isBoardAction(x, y))
                    {
                        break;
                    }
                }
                // check last visited cell is not on board
                if (!isBoardAction(x, y))
                {
                    continue;
                }
                // check if last visited cell is belong to our player ( current player )
                if (obs[x][y] == 1)
                {
                    while (true)
                    {
                        // move back until you reach starting cell
                        x -= row_direction;
                        y -= col_direction;
                        // check if this this the starting point
                        if (x == row && y == col)
                        {
                            break;
                        }
                        // else add current tile to tiles_to_flip
                        std::pair<int, int> current = std::make_pair(x, y);
                        tiles_to_flip.push_back(current);
                    }
                }
            }
        }
        obs[row][col] = 0;
        return tiles_to_flip;
    }

    std::string OthelloState::getShort()
    {
        // TODO
        throw "TODO";
    }

    std::vector<int> OthelloState::getObservation()
    {
        int player = this->observation_[2][0][0];
        std::vector<std::vector<std::vector<int8_t>>> obs;
        std::vector<int> true_obs;
        obs.reserve(2);
        true_obs.reserve(2 * 8 * 8);

        if (player == 0)
        {
            obs.emplace_back(this->observation_[0]);
            obs.emplace_back(this->observation_[1]);
            for (int channel = 0; channel < 2; channel++)
            {
                for (int row = 0; row < 8; row++)
                {
                    for (int col = 0; col < 8; col++)
                    {
                        true_obs.emplace_back(observation_[channel][row][col]);
                    }
                }
            }
        }
        else
        {
            obs.emplace_back(this->observation_[1]);
            obs.emplace_back(this->observation_[0]);

            for (int channel = 0; channel < 2; channel++)
            {
                for (int row = 0; row < 8; row++)
                {
                    for (int col = 0; col < 8; col++)
                    {
                        true_obs.emplace_back(observation_[1 - channel][row][col]);
                    }
                }
            }
        }

        int counter = 0;
        // TODO remove assert
        for (int channel = 0; channel < 2; channel++)
        {
            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    if (true_obs[counter++] != obs[channel][row][col])
                    {
                        throw "Does not equal";
                    }
                }
            }
        }
        return true_obs;
    }

    std::vector<std::tuple<common::State *, std::vector<int>>> OthelloState::getSymmerties()
    {
        // TODO
        throw "TODO";
    }

    common::ObservationSpace OthelloState::getObservationSpace()
    {
        return {2, 8, 8};
    }

    int OthelloState::getActionsCount()
    {
        return 65;
    }

    OthelloState::~OthelloState()
    {
        observation_.clear();
        actions_legality_.clear();

        observation_.shrink_to_fit();
        actions_legality_.shrink_to_fit();
    }
} // namespace games
