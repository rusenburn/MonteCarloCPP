#include <memory>
#include "gmock/gmock.h"
#include "../../src/common/player.hpp"

class MockPlayer : public common::Player
{
public:
    MOCK_METHOD(int, chooseAction, (std::shared_ptr<common::State> state_ptr), (override));
};
