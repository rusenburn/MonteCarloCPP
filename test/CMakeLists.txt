cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

set(This MonteCarloCPPTests)

set(SourceFiles 
    arena/match_test.cpp
    common/player_test.cpp
    common/game_test.cpp
)

add_executable(${This} ${SourceFiles})

target_link_libraries(${This} PUBLIC
    gmock_main
    MonteCarlo
    )

add_test(
    NAME ${This}
    COMMAND ${This}
)