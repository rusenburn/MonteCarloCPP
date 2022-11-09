#include <iostream>
#include <memory>
#include <chrono>
#include <random>
#include "../src/common/game.hpp"
#include "../src/games/othello/othello_game.hpp"
#include <time.h>
#include "../src/arena/match.hpp"
#include "../src/arena/players/random_action_player.hpp"
#include "../src/arena/players/mcts_player.hpp"
#include "../src/arena/players/mcs_player.hpp"
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
    auto d = std::chrono::duration<int, std::milli>(1000);
    auto n_sims = 1;
    // evaluators::RandomActionEvaluator p1_eval{};
    // arena::MCTSPlayer p1(&p1_eval, g.getActionsCount(), n_sims, d, 1.0f, 2.0f);

    // evaluators::RandomActionEvaluator p2_eval{};
    // d = std::chrono::duration<int, std::milli>(0);
    // arena::MCTSPlayer p2(&p2_eval, g.getActionsCount(), 100, d, 1.0f, 1.0f);

    // arena::MCSPlayer p1{g.getActionsCount(), n_sims, d, 1.0f, 4.0f};
    arena::UctPlayer p1(g.getActionsCount(), n_sims, d/2, 1.0f, 2.0f);
    arena::MCSPlayer p2{g.getActionsCount(), n_sims, d, 1.0f, 2.0f};
    arena::Match m = arena::Match(game_fn, &p1, &p2, 20, false);
    auto score = m.start();
    std::cout << "[";
    for (auto &s : score)
    {
        std::cout << s << ',';
    }
    std::cout << " ]";
}