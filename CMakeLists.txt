cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

set(This MonteCarlo)

project(${This} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")
enable_testing()

add_subdirectory(libs/googletest)


set(Headers
    src/arena/players/grave_player.hpp
    src/arena/players/human_player.hpp
    src/arena/players/mcrave_player.hpp
    src/arena/players/mcs_player.hpp
    src/arena/players/mcts_player.hpp
    src/arena/players/random_action_player.hpp
    src/arena/players/uct_player.hpp
    src/arena/match.hpp
    src/common/evaluation.hpp
    src/common/evaluator.hpp
    src/common/game_result.hpp
    src/common/game.hpp
    src/common/observation_space.hpp
    src/common/player.hpp
    src/common/search_tree.hpp
    src/common/state.hpp
    src/common/terminal_status.hpp
    src/evaluators/random_action_evaluator.hpp
    src/games/othello/othello_game.hpp
    src/games/othello/othello_state.hpp
    src/searchTrees/grave/grave_node.hpp
    src/searchTrees/grave/grave.hpp
    src/searchTrees/mcrave/mcrave_node.hpp
    src/searchTrees/mcrave/mcrave.hpp
    src/searchTrees/mcs/mcs.hpp
    src/searchTrees/monteCarlo/mcts.hpp
    src/searchTrees/monteCarlo/node.hpp
    src/searchTrees/uct/uct_node.hpp
    src/searchTrees/uct/uct.hpp
    
)

set(SourceFiles
    src/arena/players/grave_player.cpp
    src/arena/players/human_player.cpp
    src/arena/players/mcrave_player.cpp
    src/arena/players/mcs_player.cpp
    src/arena/players/mcts_player.cpp
    src/arena/players/random_action_player.cpp
    src/arena/players/uct_player.cpp
    src/arena/match.cpp
    src/evaluators/random_action_evaluator.cpp
    src/games/othello/othello_game.cpp
    src/games/othello/othello_state.cpp
    src/searchTrees/grave/grave_node.cpp
    src/searchTrees/grave/grave.cpp
    src/searchTrees/mcrave/mcrave_node.cpp
    src/searchTrees/mcrave/mcrave.cpp
    src/searchTrees/mcs/mcs.cpp
    src/searchTrees/monteCarlo/mcts.cpp
    src/searchTrees/monteCarlo/node.cpp
    src/searchTrees/uct/uct_node.cpp
    src/searchTrees/uct/uct.cpp
)


add_library(${This} STATIC ${SourceFiles} ${Headers})

add_subdirectory(test)
add_subdirectory(app)
