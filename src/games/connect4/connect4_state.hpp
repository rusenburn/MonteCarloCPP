#ifndef GAMES_CONNECT4_STATE_HPP_
#define GAMES_CONNECT4_STATE_HPP_
#include <array>
#include <vector>
#include "../../common/state.hpp"

namespace games
{

    class Connect4State : public common::State
    {
    private:
        const static int N_COLS = 7;
        const static int N_ROWS = 6;
        std::array<std::array<int, N_COLS>, N_ROWS> observation_;
        std::vector<int> actions_legality_;
        int turn_;
        int last_action_;

        bool is_terminal_cached_;
        bool is_terminal_;
        bool is_game_result_cached_;
        common::GameResult game_result_;
        bool isLegalAction(int action);
        bool isWinning(int player);
        bool isFull();
        bool isVerticalWin(int player, int row, int col);
        bool isHorizontalWin(int player, int row, int col);
        bool isForwardDiagonalWin(int player, int row, int col);
        bool isBackwarDiagonalWin(int player, int row, int col);

    public:
        Connect4State(std::array<std::array<int, N_COLS>, N_ROWS> observation, int last_action, int turn);

        std::vector<int> getActionsLegality() override;

        common::ObservationSpace getObservationSpace() override;

        int getActionsCount() override;

        bool isTerminal() override;

        common::GameResult getResult() override;

        Connect4State *step(int action) override;

        std::vector<int> getObservation() override;

        std::vector<std::tuple<State *, std::vector<int>>> getSymmerties() override;

        std::string getShort() override;

        void render() override;
    };
} // namespace games

#endif