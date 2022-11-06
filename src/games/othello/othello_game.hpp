#ifndef GAMES_OTHELLO_GAME_HPP_
#define GAMES_OTHELLO_GAME_HPP_

#include <memory>
#include <vector>
#include "othello_state.hpp"
#include "../../common/game.hpp"
#include "../../common/state.hpp"

namespace games
{
    class OthelloGame : public common::Game
    {
    private:
        OthelloState *state_ptr_;
        OthelloState *getNewState();

    public:
        OthelloGame();
        int getActionsCount() override;
        common::ObservationSpace getObservationSpace() override;
        OthelloState *reset() override;
        OthelloState *step(int action) override;
        void render() override;
        bool isTerminal() override;
    };
} // namespace games

#endif