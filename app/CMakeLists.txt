cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

set(This app)


set(Headers
    console.hpp
)

set(SourceFiles 
    main.cpp
    console.cpp
)

add_executable(${This} ${SourceFiles})

target_link_libraries(${This} PUBLIC
    MonteCarlo
    )

add_test(
    NAME ${This}
    COMMAND ${This}
)