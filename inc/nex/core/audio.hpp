#pragma once

#include "asset.hpp"
#include "defines.hpp"

#include <string>

namespace NEX::Core
{
    struct Audio : public IAsset
    {
        MIX_Audio* audio = nullptr;
        MIX_Track* track = nullptr;
        std::string path;

        Audio(const std::string&, bool = true);
        ~Audio();
    };

    struct AudioManager
    {
        AudioManager() = delete;

        static MIX_Mixer* get_mixer();

      private:
        struct AudioManagerInner
        {
            MIX_Mixer* mixer;

            AudioManagerInner();
            ~AudioManagerInner();
        };

        static AudioManagerInner& get_instance();
    };
}; // namespace NEX::Core
