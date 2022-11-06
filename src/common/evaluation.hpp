#ifndef COMMON_EVALUATION_HPP_
#define COMMON_EVALUATION_HPP_

#include <vector>

namespace common
{
    struct Evaluation
    {
        std::vector<float> action_probs;
        std::vector<float> wdl;
    };
} // namespace common

#endif