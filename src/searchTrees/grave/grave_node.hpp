#ifndef SEARCH_TREES_GRAVE_GRAVE_NODE_HPP_
#define SEARCH_TREES_GRAVE_GRAVE_NODE_HPP_

#include "../../common/state.hpp"
#include "../../common/terminal_status.hpp"
namespace searchTrees
{
    class GraveNode
    {
    private:
        common::State *state_ptr_;
        int n_game_actions_;
        common::TerminalStatus terminal_;
        bool is_leaf_node_;
        bool is_game_result_cached_;
        common::GameResult game_result_;

        std::vector<GraveNode *> children_;

        int n_;
        // total score for action "a"
        std::vector<float> wsa_;

        // total visits for action "a"
        std::vector<int> nsa_;

        // player 0 amaf total score for action "a"
        std::vector<float> amaf_wsa_player_0;

        // player 0 amaf total visits for action "a"
        std::vector<int> amaf_nsa_player_0;

        // player 1 amaf total score for action "a"
        std::vector<float> amaf_wsa_player_1;

        // player 1 amaf total score for action "a"
        std::vector<int> amaf_nsa_player_1;

        std::vector<int> actions_legality_;
        void heuristic();

    protected:
        // named playout in the paper
        virtual float playout(std::vector<int> &out_our_actions, std::vector<int> &out_opponent_actions);

    public:
        GraveNode(common::State *state_ptr, int n_game_actions);
        ~GraveNode();
        float simulateOne(GraveNode *amaf_node_ptr,
                          bool save_illegal_amaf_actions,
                          int depth,
                          std::vector<int> &out_our_actions,
                          std::vector<int> &out_opponent_actions,
                          const int &amaf_min_ref_count,
                          const float &b_square_ref);
        int selectMove(GraveNode *amaf_node_ptr, int depth,const float &b_square_ref);
    };

} // namespace searchTrees

#endif
