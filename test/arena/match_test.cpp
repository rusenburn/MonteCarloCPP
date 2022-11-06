#include <memory>
#include "gmock/gmock.h"
#include "../../src/arena/match.hpp"
#include "../../src/common/game.hpp"
#include "../../src/common/state.hpp"
#include "../common/player_test.cpp"
#include "../common/game_test.cpp"

std::shared_ptr<MockGame> mocked_game = std::make_shared<MockGame>();

std::shared_ptr<common::Game> game_fn()
{
    return mocked_game;
}

class MatchTests : public ::testing::Test
{

protected:
    std::shared_ptr<MockPlayer> mocked_player_1_ptr;
    std::shared_ptr<MockPlayer> mocked_player_2_ptr;
    MatchTests()
        : mocked_player_1_ptr(std::make_shared<MockPlayer>()),
          mocked_player_2_ptr(std::make_shared<MockPlayer>())
    {
    }
    void SetUp() override
    {
        mocked_game = std::make_shared<MockGame>();
    }
    void TearDown() override {}
};

TEST_F(MatchTests, CanInstantiate)
{
    arena::Match service_under_test(game_fn, mocked_player_1_ptr, mocked_player_2_ptr, 1, false);
}

TEST_F(MatchTests, CAN)
{
    // arrange
    arena::Match service_under_test(game_fn, mocked_player_1_ptr, mocked_player_2_ptr, 1, false);

    // act
    // std::vector<int> result = service_under_test.start();

    // assert
}