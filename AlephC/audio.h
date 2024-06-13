#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <iostream>
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

class AudioSys
{
public:
    ma_engine engine;
    ma_result engineRes;
    
    AudioSys()
    {
        engineRes = ma_engine_init(nullptr, &engine);
        if (engineRes != MA_SUCCESS) std::cout << "Failed to init sound system\n";
        
    }

    void play_track(const std::vector<const char*> &track_list)
    {
        for (const char* path : track_list)
        {
            ma_engine_play_sound(&engine, path, nullptr);
        }
    }

    void play_single_sound(const char* soundPath)
    {
        ma_engine_play_sound(&engine, soundPath, nullptr);
    }
};
#endif