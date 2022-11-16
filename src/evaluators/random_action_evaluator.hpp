#ifndef EVALUATORS_RANDOM_ACTION_EVALUATOR_HPP_
#define EVALUATORS_RANDOM_ACTION_EVALUATOR_HPP_

#include "../common/evaluator.hpp"
#include "../common/evaluation.hpp"
namespace evaluators
{
    class RandomActionEvaluator : public common::Evaluator
    {
    public:
        common::Evaluation evaluate(common::State *state_ptr) override;
        ~RandomActionEvaluator(){};
    };
} // namespace evaluators

#endif