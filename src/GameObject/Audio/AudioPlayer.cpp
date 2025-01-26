#include "AudioPlayer.hpp"
#include "../../TimeTracker.hpp"
#include "../Scene/Game.hpp"

/*
 * GameObject that plays audio. 
 * 
 * \note Currently, audio may only be played starting form the beginning of the track.
 * 
 * \warning for self destruction to work properly, it is reccomended to add
 * AudioPlayer objects into the scene directly (i.e. with `Instantiate<AudioPlayer>(...)`), 
 * as opposed to adding them as a child to some parent object. 
 * 
 * \param sound A pointer to the actual audio that will get played.
 * \param playOnStart Indicate if the audio should start playing upon instantiation.
 * \param destroyOnEnd Indicate if the AudioPlayer should destroy itself when audio finishes playing.
 */
AudioPlayer::AudioPlayer(std::shared_ptr<LAudio> sound, bool playOnStart, bool destroyOnEnd)
: GameObject(), audio(sound), terminate(destroyOnEnd)
{
    if (playOnStart) Play();
}

AudioPlayer::~AudioPlayer()
{
    Destroy();
}

void AudioPlayer::Destroy()
{
    GameObject::Destroy();
    audio = nullptr;
    s = nullptr;
}

/* This is just to access self as a shared pointer for self destruction */
void AudioPlayer::AssignComponents(std::shared_ptr<GameObject> self)
{
    s = self;
}

/* 
 * If the audio is playing, counts down the playback timer. If the timer
 * reaches 0, stop playing. If the object is set ot destroy itself on termination, 
 * destroy the audioplayer object.
 */
void AudioPlayer::Update()
{
    if (playing) {
        // count down timer
        time_remaining -= gTime.deltaTime();
        // audio done playing
        if (time_remaining<=0.0f) {
            if (terminate) DestroyObject(s); // kill yourself in minecraft
            else playing = false; // stop playing
        }
    }
}

/* 
 * Begins playing audio from the beginning.
 *
 * \param channel The audio channel the audio should be played on. 
 * Leave as the default parameter (-1) to automatically choose a free channel.
 * \param loops the number of times the audio will loop after the first play.
 * Leave as `0` to play the audio once, set to `1` to have the audio play twice, 
 * and so on. Set this value to `-1` to have the audio loop indefinitely 
 * (until `Halt()` is called).
 */
void AudioPlayer::Play(int channel, int loops)
{
    // determine how long the duration is based on loop count.
    // for infinite loops, just make the value positive
    time_remaining = (loops<0)? 1.0f : audio->duration()*(loops+1);
    // play the audio
    audio->play(channel, loops);
    // for infinite loops, don't enable playing, so that it won't be destroyed. 
    if (loops<0) playing = true;
}

/* Stop playing audio. */
void AudioPlayer::Halt()
{
    time_remaining = 0.0f;
    playing = false;
    audio->stop();
}

/* Returns the duration (in seconds) of the stored audio */
float AudioPlayer::Duration() const { return audio->duration(); }
/* Returns how much time is left in the current playback of the audio. */
float AudioPlayer::timeRemaining() const { return time_remaining; }

/* 
 * Specify whether the audio object should destroy itself after finishing 
 * playing its audio. This is reccomended for one time use audio players, such
 * as spontaneous sound effects. 
 */
void AudioPlayer::DestroyOnFinish(bool destroy) { terminate = destroy; }