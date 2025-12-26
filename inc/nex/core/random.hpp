#pragma once

#include <algorithm>
#include <concepts>
#include <random>

namespace NEX::Core
{
    // I stole this, don't cite me. LGTM.

    struct Random
    {
      public:
        template <std::integral Int> static Int range(Int min, Int max)
        {
            thread_local std::mt19937_64 rng{std::random_device{}()};
            std::uniform_int_distribution<Int> dist(min, max);
            return dist(rng);
        }

        template <std::floating_point FP> static FP range(FP min, FP max)
        {
            thread_local std::mt19937_64 rng{std::random_device{}()};
            std::uniform_real_distribution<FP> dist(min, max);
            return dist(rng);
        }

        template <class Container> static const auto &choice(const Container &c)
        {
            return c[range<std::size_t>(0, c.size() - 1)];
        }

        template <class Container> static void shuffle(Container &c)
        {
            thread_local std::mt19937_64 rng{std::random_device{}()};
            std::shuffle(c.begin(), c.end(), rng);
        }
    };
} // namespace NEX::Core
