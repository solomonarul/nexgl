#pragma once

#include <cstdint>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

namespace NEX::Core
{
    class DeltaTimer
    {
      public:
        DeltaTimer();
        double tick();

      private:
        uint64_t last;
    };

    struct IAppState
    {
        bool running = true;
    };

    const auto RUN_MAIN_LOOP = [](bool loop_function(double time, void* userdata), IAppState* userdata)
    {
#ifdef __EMSCRIPTEN__
        emscripten_request_animation_frame_loop(loop_function, userdata);
#else
        NEX::Core::DeltaTimer dt;
        while (userdata->running) loop_function(dt.tick(), userdata);
#endif
    };
}; // namespace NEX::Core
