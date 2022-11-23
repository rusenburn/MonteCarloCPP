#ifndef GAMES_CONNECT4_GAME_HPP_
#define GAMES_CONNECT4_GAME_HPP_

#include "../../common/game.hpp"
#include "connect4_state.hpp"
namespace games
{
    class Connect4Game : public common::Game
    {
    private:
        Connect4State *state_ptr;
        Connect4State* getNewState();

    public:
        Connect4Game();

        int getActionsCount() override;

        
        common::ObservationSpace getObservationSpace() override;

        
        Connect4State *reset() override;

        
        Connect4State *step(int action) override;

        
        bool isTerminal() override;

        
        void render() override;

        ~Connect4Game();
    };
} // namespace games

#endif
