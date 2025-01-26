#ifndef LAUDIO_HPP
#define LAUDIO_HPP

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>

#define NUM_AVAILABLE_CHANNELS 8

class LAudio
{
    public:
        // initialises variables
        LAudio();

        // deallocates memory
        ~LAudio();

        // loads audio from an mp3 file
        bool loadFromFile(std::string path);

        // deallocates audio
        void free();

        // plays the audio
        void play( int Channel = -1, int loops = 0 );

        // halts the audio
        void stop();

        float duration();

    private:

        // the actual audio
        Mix_Chunk *chunk = NULL;

        int channel = -1;

        /* The duration of the audio (in seconds) */
        float len = 0.0f;

        Uint32 getChunkTimeMilliseconds();
};

#endif