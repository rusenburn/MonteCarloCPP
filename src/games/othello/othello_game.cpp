#include "othello_game.hpp"
#include "othello_state.hpp"
namespace games
{
    OthelloGame::OthelloGame()
        : state_ptr_(getNewState())
    {
    }

    int OthelloGame::getActionsCount()
    {
        return state_ptr_->getActionsCount();
    }

    common::ObservationSpace OthelloGame::getObservationSpace()
    {
        return state_ptr_->getObservationSpace();
    };

    OthelloState *OthelloGame::reset()
    {
        state_ptr_ = this->getNewState();
        return state_ptr_;
    };

    OthelloState *OthelloGame::step(int action)
    {
        state_ptr_ = state_ptr_->step(action);
        return state_ptr_;
    };

    void OthelloGame::render()
    {
        state_ptr_->render();
    };

    bool OthelloGame::isTerminal()
    {
        return state_ptr_->isTerminal();
    }

    OthelloState *OthelloGame::getNewState()
    {
        const int N_ROWS = 8;
        const int N_COLS = 8;
        const int N_PLAYERS = 2;
        std::vector<std::vector<std::vector<int8_t>>> obs;
        obs.reserve(N_PLAYERS + 1);
        for (int p = 0; p < N_PLAYERS; p++)
        {
            obs.emplace_back(std::vector<std::vector<int8_t>>());
            obs[p].reserve(N_ROWS);
            for (int row = 0; row < N_ROWS; row++)
            {
                obs[p].emplace_back(std::vector<int8_t>());
                obs[p][row].reserve(N_COLS);
                for (int col = 0; col < N_COLS; col++)
                {
                    obs[p][row].emplace_back(0);
                }
            }
        }
        obs[1][3][3] = 1;
        obs[1][4][4] = 1;
        obs[0][3][4] = 1;
        obs[0][4][3] = 1;

        int p = 0;
        obs.emplace_back(std::vector<std::vector<int8_t>>());
        obs[2].reserve(N_ROWS);
        for (int row = 0; row < N_ROWS; row++)
        {
            obs[2].emplace_back(std::vector<int8_t>());
            obs[2][row].reserve(N_COLS);
            for (int col = 0; col < N_COLS; col++)
            {
                obs[2][row].emplace_back(0);
            }
        }
        OthelloState *state_ptr = new OthelloState(obs, 0);
        return state_ptr;
    }
} // namespace games
