#include "core/audio.hpp"

#include <cstdlib>

#ifdef ASSET_LOAD_LOG
#include "core/defines.hpp"

#include <iostream>
#endif

using namespace NEX::Core;

Audio::Audio(const std::string& path, bool predecode)
{
    MIX_Mixer* mixer = AudioManager::get_mixer();
    this->audio = MIX_LoadAudio(mixer, path.c_str(), predecode);
    this->track = MIX_CreateTrack(mixer);
    MIX_SetTrackAudio(this->track, this->audio);
    this->path = path;

#ifdef ASSET_LOAD_LOG
    std::cout << TTY_BLUE << "[INFO] Loaded sound (path: " << path << ")\n" << TTY_RESET;
#endif
}

Audio::~Audio()
{
    if (this->track)
    {
        MIX_PauseTrack(this->track);
        MIX_DestroyTrack(this->track);
        this->track = nullptr;
    }
    if (this->audio)
        MIX_DestroyAudio(this->audio), this->audio = nullptr;

#ifdef ASSET_LOAD_LOG
    std::cout << TTY_BLUE << "[INFO] Destroyed sound (path: " << this->path << ")\n" << TTY_RESET;
#endif
}

void Audio::play_audio(void)
{
    MIX_PlayAudio(AudioManager::get_mixer(), this->audio);
}

void Audio::play_track(SDL_PropertiesID options)
{
    MIX_PlayTrack(this->track, options);
}

void Audio::set_gain_track(double ammount)
{
    MIX_SetTrackGain(this->track, ammount);
}

bool Audio::is_playing_track(void)
{
    return MIX_TrackPlaying(this->track);
}

void Audio::stop_track(Sint64 fade_out_frames)
{
    MIX_StopTrack(this->track, fade_out_frames);
}

MIX_Mixer* AudioManager::get_mixer()
{
    return get_instance().mixer;
}

AudioManager::AudioManagerInner& AudioManager::get_instance()
{
    static AudioManagerInner instance;
    return instance;
}

void AudioManager::stop_all_tracks(int64_t fade_out_ms)
{
    MIX_StopAllTracks(AudioManager::get_mixer(), fade_out_ms);
}

AudioManager::AudioManagerInner::AudioManagerInner()
{
    if (!MIX_Init())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Could not init SDL3 mixer!", SDL_GetError(), NULL);
        exit(-1);
    }

    SDL_AudioSpec spec{.format = SDL_AUDIO_S16, .channels = 2, .freq = 44100};
    this->mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
}

AudioManager::AudioManagerInner::~AudioManagerInner()
{
    MIX_DestroyMixer(this->mixer);
    MIX_Quit();
}
