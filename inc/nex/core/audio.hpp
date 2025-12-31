#pragma once

#include "asset.hpp"
#include "defines.hpp"

#include <string>

namespace NEX::Core
{
    struct Audio : public IAsset
    {
        MIX_Audio* audio = nullptr;
        MIX_Track* track = nullptr; // TODO: separate the track from the audio perhaps?
        std::string path;

        Audio(const std::string&, bool = true);
        ~Audio();

        void play_audio(void);
        void play_track(SDL_PropertiesID = 0);
        void set_gain_track(double);
        bool is_playing_track(void);
    };

    struct AudioManager
    {
        AudioManager() = delete;

        static MIX_Mixer* get_mixer();
        static void stop_all_tracks(int64_t = 0);

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
