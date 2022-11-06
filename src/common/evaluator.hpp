#ifndef COMMON_EVALUATOR_HPP_
#define COMMON_EVALUATOR_HPP_

#include <memory>
#include <vector>

#include "evaluation.hpp"
#include "state.hpp"

namespace common
{
    class Evaluator
    {
    public:
        virtual Evaluation evaluate(State *state_ptr) = 0;
    };
} // namespace common

#endif
