#ifndef APP_CONSOLE_HPP_
#define APP_CONSOLE_HPP_

#include <memory>

#include "../src/common/player.hpp"
#include "../src/common/game.hpp"
#include "../src/games/othello/othello_game.hpp"
namespace console
{

    class Console
    {

    public:
        void run();

    private:
        void run_console();
        void edit_settings();
        void change_game();
        void change_n_sims();
        void change_duration();
        void change_n_sets();
        common::Player *get_player_ptr(int player_num, bool is_default);
        common::Player *create_human_player();
        common::Player *create_default_grave_player();
        common::Player *create_default_mcrave_player();
        common::Player *create_default_mcs_player();
        common::Player *create_default_mcts_player();
        common::Player *create_random_action_player();
        common::Player *create_default_utc_player();

        static std::shared_ptr<common::Game> othello_game_fn();
        int min_duration_in_millis = 500;
        int min_simulations = 10;

        char game_name[50] = "Othello";
        int n_game_actions = 65;

        char player_1_name[50] = "G-rave";
        char player_2_name[50] = "G-rave";

        int match_n_sets = 10;
        bool match_render = false;

        common::Player *player_1_ptr = nullptr;
        common::Player *player_2_ptr = nullptr;

        std::shared_ptr<common::Game> (*game_fn_ptr)() = othello_game_fn;
    };
} // namespace console

#endif // APP_CONSOLE_HPP_
