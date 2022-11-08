#include <memory>
#include "gmock/gmock.h"
#include "../../src/common/player.hpp"

class MockPlayer : public common::Player
{
public:
    MOCK_METHOD(int, chooseAction, (common::State * state_ptr), (override));
};
