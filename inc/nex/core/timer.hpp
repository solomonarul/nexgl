#pragma once

namespace NEX::Core
{
    struct Timer
    {
        bool ok = false;
        double value = 0, rate;
        Timer(double);

        float progress(void);
        void update(double);
        bool has_ticked(void);
    };
}; // namespace NEX::Core
