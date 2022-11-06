#include "gmock/gmock.h"
#include "../../src/common/game.hpp"
#include "../../src/common/observation_space.hpp"
class MockGame : public common::Game
{
public:
    MOCK_METHOD(int, getActionsCount, (), (override));
    MOCK_METHOD(common::ObservationSpace, getObservationState, (), (override));
    MOCK_METHOD(std::shared_ptr<common::State>, reset, (), (override));
    MOCK_METHOD(std::shared_ptr<common::State>, step, (int action), (override));
    MOCK_METHOD(bool, isTerminal, (), (override));
    MOCK_METHOD(void, render, (), (override));
};