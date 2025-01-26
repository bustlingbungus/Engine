#include "LAudio.hpp"
#include <iostream>

int g_channel = 0;

LAudio::LAudio()
{
    // open that audio device for playback
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
}

LAudio::~LAudio()
{
    free();
}

void LAudio::free()
{
    if (chunk != NULL) {
        Mix_FreeChunk(chunk);
        chunk = NULL;
        len = 0.0f;
    }
}

bool LAudio::loadFromFile(std::string path)
{
    // get rid of preexisting audio
    free();

    // load the audio from file
    Mix_Chunk *newChunk = Mix_LoadWAV(path.c_str());

    if (newChunk == NULL) 
    {
        printf("Unable to create audio from %s! SDL Error: %s\n", path.c_str(), Mix_GetError());
    }

    // return success
    chunk = newChunk;
    len = (float)getChunkTimeMilliseconds() / 1000.0f;
    return chunk != NULL;
}

void LAudio::play( int Channel, int loops )
{
    if (Channel == -1) 
    {
        // channel 0 reserved for music
        if (g_channel == 0) g_channel++;

        // find a channel to play the audio on
        channel = g_channel;
        g_channel = (g_channel+1) % NUM_AVAILABLE_CHANNELS;
    }
    else
    {
        if (g_channel == Channel) g_channel = (g_channel+1) % NUM_AVAILABLE_CHANNELS;
        channel = Channel;
    }

    Mix_PlayChannel(channel, chunk, loops);
}

void LAudio::stop()
{
    if (channel != -1) Mix_HaltChannel(channel);
    channel = -1;
}

/* Returns the duration of the audio in seconds. */
float LAudio::duration() { return len; }

Uint32 LAudio::getChunkTimeMilliseconds()
{
    if (chunk != NULL)
    {
        Uint32 points = 0, frames = 0;
        int freq = 0, chans = 0;
        Uint16 fmt = 0;

        // error handling
        if (!Mix_QuerySpec(&freq, &fmt, &chans)) return 0;

        // smaple points = bytes / sample size
        points = chunk->alen / ((fmt&0xFF) / 8);
        // sample frames = points / channels
        frames = points / chans;
        // duration (in ms) = (1000*frames) / frequency
        return (frames*1000)/freq;
    }
    return 0;
}