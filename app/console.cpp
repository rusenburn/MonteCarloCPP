#include <iostream>
#include <sstream>

#include <chrono>

#include "../src/arena/players/grave_player.hpp"
#include "../src/arena/players/human_player.hpp"
#include "../src/arena/players/mcrave_player.hpp"
#include "../src/arena/players/mcs_player.hpp"
#include "../src/arena/players/mcts_player.hpp"
#include "../src/arena/players/uct_player.hpp"
#include "../src/evaluators/random_action_evaluator.hpp"
#include "../src/arena/players/random_action_player.hpp"
#include "../src/arena/match.hpp"
#include "../src/games/othello/othello_game.hpp"
#include "../src/games/connect4/connect4_game.hpp"
#include "console.hpp"

namespace console
{

    void Console::run()
    {
        using namespace std;
        cout << endl;
        cout << "Do you want to run default settings? [Y/n] ";

        char y_n[50];
        y_n[0] = 'Y';
        cin >> y_n;
        cout << '\n';
        if (y_n[0] == 'n' || y_n[0] == 'N')
        {
            edit_settings();
        }
        run_console();
    }

    void Console::run_console()
    {
        time_t a = std::time(0);
        srand(a);

        std::shared_ptr<common::Game> game_ptr = game_fn_ptr();
        player_1_ptr = player_1_ptr == nullptr ? get_player_ptr(1, true) : player_1_ptr;
        player_2_ptr = player_2_ptr == nullptr ? get_player_ptr(2, true) : player_2_ptr;

        arena::Match m(game_fn_ptr, player_1_ptr, player_2_ptr, match_n_sets, match_render);

        std::cout << "Starting match..." << std::endl;
        auto score = m.start();
        std::cout << "[";
        for (auto &s : score)
        {
            std::cout << s << ',';
        }
        std::cout << " ]";
    }

    void Console::edit_settings()
    {
        using namespace std;
        int action = 0;
        bool done = false;
        do
        {
            cout.clear();
            cout << "Pick a choice:" << '\n';

            cout << "[ 1 ] Change game. (current = " << game_name << " )" << '\n';
            cout << "[ 2 ] Change minimum number of simulations for AI. (current = " << min_simulations << " )" << '\n';
            cout << "[ 3 ] Change turn duration in millis for AI. (current = " << min_duration_in_millis << " )" << '\n';
            cout << "[ 4 ] Change number of sets. (current = " << match_n_sets << " )" << '\n';
            cout << "[ 0 ] Done " << '\n';
            cin >> action;
            cout << endl;
            switch (action)
            {
            case 1:
                change_game();
                break;
            case 2:
                change_n_sims();
                break;
            case 3:
                change_duration();
                break;
            case 4:
                change_n_sets();
                break;
            case 0:
                done = true;
                break;
            default:
                break;
            }
        } while (!done);

        player_1_ptr = get_player_ptr(1, false);
        player_2_ptr = get_player_ptr(2, false);
    }

    void Console::change_game()
    {
        using namespace std;
        bool done = false;
        int choice = 0;
        do
        {
            cout.clear();
            cout << "Choose a game:" << '\n';
            cout << "[ 1 ] Othello." << '\n';
            cout << "[ 2 ] Connect4." << '\n';
            cin >> choice;
            cout << endl;
            switch (choice)
            {
            case 1:
                game_fn_ptr = othello_game_fn;
                break;
            case 2:
                memcpy(game_name,"Connect4",9);
                game_fn_ptr = connect4_game_fn;
                break;
            default:
                break;
            }
            done = true;
        } while (!done);
        cout << endl;
        n_game_actions = game_fn_ptr()->getActionsCount();
    }

    void Console::change_n_sims()
    {
        std::cout.clear();
        std::cout << "Set minimum number of sims per turn for AI ( current = " << min_simulations << " ): " << '\n';
        std::cin >> min_simulations;
        std::cout << std::endl;
    }

    void Console::change_duration()
    {
        std::cout.clear();
        std::cout << "Set minimum turn duration in millis for AI ( current = " << min_duration_in_millis << " ): " << '\n';
        std::cin >> min_duration_in_millis;
        std::cout << std::endl;
    }

    void Console::change_n_sets()
    {
        std::cout.clear();
        std::cout << "Set  number of sets. ( current = " << match_n_sets << " ): " << '\n';
        std::cin >> match_n_sets;
        std::cout << std::endl;
    }

    common::Player *Console::get_player_ptr(int player_num, bool is_default)
    {
        if (is_default)
        {
            return create_default_grave_player();
        }
        std::cout.clear();
        bool done = false;
        int choice = 0;
        std::chrono::duration<int, std::milli>(min_duration_in_millis);
        do
        {
            std::cout << "Set Player " << player_num << "( current = " << (player_num == 1 ? player_1_name : player_2_name) << '\n';
            std::cout << "[ 1 ] Human" << '\n';
            std::cout << "[ 2 ] G-rave player" << '\n';
            std::cout << "[ 3 ] MC-rave player" << '\n';
            std::cout << "[ 4 ] Monte carlo search player" << '\n';
            std::cout << "[ 5 ] Monte carlo Tree search player" << '\n';
            std::cout << "[ 6 ] UCT player" << '\n';
            std::cout << "[ 7 ] Random action player" << '\n';
            std::cout << "[ 0 ] Done" << '\n';
            std::cin >> choice;
            std::cout << std::endl;

            switch (choice)
            {
            case 1:
                return create_human_player();
                break;
            case 2:
                return create_default_grave_player();
                break;
            case 3:
                return create_default_mcrave_player();
                break;
            case 4:
                return create_default_mcs_player();
                break;
            case 5:
                return create_default_mcts_player();
                break;
            case 6:
                return create_default_utc_player();
                break;
            case 7:
                return create_random_action_player();
                break;
            default:
                return create_default_grave_player();
                break;
            }
        } while (!done);

        return create_default_grave_player();
    }

    common::Player *Console::create_human_player()
    {
        return new arena::HumanPlayer();
    }
    common::Player *Console::create_default_grave_player()
    {
        std::chrono::duration<int, std::milli> d(min_duration_in_millis);
        return new arena::GravePlayer(n_game_actions, min_simulations, d, 15, 0.04f, true);
    }
    common::Player *Console::create_default_mcrave_player()
    {
        std::chrono::duration<int, std::milli> d(min_duration_in_millis);
        return new arena::McRavePlayer(n_game_actions, min_simulations, d, 0.1f);
    }
    common::Player *Console::create_default_mcs_player()
    {
        std::chrono::duration<int, std::milli> d(min_duration_in_millis);
        return new arena::MCSPlayer(n_game_actions, min_simulations, d, 1.0f, 1.0f);
    }
    common::Player *Console::create_default_mcts_player()
    {
        std::chrono::duration<int, std::milli> d(min_duration_in_millis);
        // Fix this pointer
        evaluators::RandomActionEvaluator *eval_ptr = new evaluators::RandomActionEvaluator();
        return new arena::MCTSPlayer(eval_ptr, n_game_actions, min_simulations, d, 1.0f, 1.0f);
    }
    common::Player *Console::create_random_action_player()
    {
        return new arena::RandomActionPlayer(n_game_actions);
    }
    common::Player *Console::create_default_utc_player()
    {
        std::chrono::duration<int, std::milli> d(min_duration_in_millis);
        return new arena::UctPlayer(n_game_actions, min_simulations, d, 1.0f, 1.0f);
    }
    std::shared_ptr<common::Game> Console::othello_game_fn()
    {
        return std::make_shared<games::OthelloGame>();
    }

    std::shared_ptr<common::Game> Console::connect4_game_fn(){
        return std::make_shared<games::Connect4Game>();
    }
} // namespace console
