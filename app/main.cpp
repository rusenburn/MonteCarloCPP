#include <time.h>

#include <iostream>
#include <memory>
#include <chrono>
#include <random>
#include "../src/common/game.hpp"
#include "../src/games/othello/othello_game.hpp"
#include "../src/arena/match.hpp"
#include "../src/arena/players/random_action_player.hpp"
#include "../src/arena/players/mcts_player.hpp"
#include "../src/arena/players/mcs_player.hpp"
#include "../src/arena/players/mcrave_player.hpp"
#include "../src/arena/players/grave_player.hpp"
#include "../src/evaluators/random_action_evaluator.hpp"
#include "../src/arena/players/human_player.hpp"
#include "../src/arena/players/uct_player.hpp"
std::shared_ptr<common::Game> game_fn()
{
    return std::make_shared<games::OthelloGame>();
}

int main()
{
    auto a = std::time(0);
    srand(a);
    auto g = games::OthelloGame();
    // arena::RandomActionPlayer p1{g.getActionsCount()};
    // arena::HumanPlayer p1{};
    auto d = std::chrono::duration<int, std::milli>(500);
    auto n_sims = 1;
    // arena::McRavePlayer p1(g.getActionsCount(), n_sims, d,0.12f);
    // arena::MCSPlayer p1{g.getActionsCount(), n_sims, d, 0, 1.0f};
    arena::UctPlayer p1(g.getActionsCount(), n_sims, d, 1.0f, 1.0f);
    // evaluators::RandomActionEvaluator p1_eval{};
    // arena::MCTSPlayer p1(&p1_eval, g.getActionsCount(), n_sims, d, 1.0f, 1.0f);

    // arena::GravePlayer p1(g.getActionsCount(), n_sims, d, 13, 0.000005f, true);
    // d = std::chrono::duration<int, std::milli>(0);
    // evaluators::RandomActionEvaluator p2_eval{};
    // arena::MCTSPlayer p2(&p2_eval, g.getActionsCount(), n_sims, d, 1.0f, 1.0f);

    // arena::MCSPlayer p2{g.getActionsCount(), n_sims, d, 1.0f, 2.0f};
    // arena::McRavePlayer p2(g.getActionsCount(), n_sims, d, 0.1f);
    arena::GravePlayer p2(g.getActionsCount(), n_sims, d, 10, 0.05f, true);
    // arena::RandomActionPlayer p2(g.getActionsCount());
    arena::Match m = arena::Match(game_fn, &p1, &p2, 10, false);
    auto score = m.start();
    std::cout << "[";
    for (auto &s : score)
    {
        std::cout << s << ',';
    }
    std::cout << " ]";
}