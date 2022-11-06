#ifndef GAMES_OTHELLO_STATE_HPP_
#define GAMES_OTHELLO_STATE_HPP_

#include <memory>
#include <vector>

#include "../../common/state.hpp"
#include "../../common/game_result.hpp"
#include "../../common/observation_space.hpp"
namespace games
{
    class OthelloState : public common::State
    {
    private:
        std::vector<std::vector<std::vector<int8_t>>> observation_;
        int n_consecutive_skips_;
        std::vector<int> actions_legality_;
        std::vector<std::pair<int, int>> getBoardChangesOnAction(int row, int col);
        bool isBoardAction(int row, int col);

    public:
        OthelloState(std::vector<std::vector<std::vector<int8_t>>> observation, int consecutive_skips);
        std::vector<int> getActionsLegality() override;
        bool isTerminal() override;
        common::GameResult getResult() override;
        OthelloState *step(int action) override;
        std::vector<int> getObservation() override;
        void render() override;
        std::vector<std::tuple<common::State *, std::vector<int>>> getSymmerties() override;
        std::string getShort() override;
        common::ObservationSpace getObservationSpace() override;
        int getActionsCount() override;
        ~OthelloState();
    };
} // namespace games

#endif
