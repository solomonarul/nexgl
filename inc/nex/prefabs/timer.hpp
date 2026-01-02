#pragma once

namespace NEX::Prefabs
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
}; // namespace NEX::Prefabs
