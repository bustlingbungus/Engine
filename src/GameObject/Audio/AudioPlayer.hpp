#pragma once

#include "../GameObject.hpp"
#include "../../sdl/LAudio.hpp"

/* Game object that plays audio. */
class AudioPlayer : public GameObject
{
    public:

        AudioPlayer(std::shared_ptr<LAudio> sound, bool playOnStart = false, bool destroyOnEnd = false);
        ~AudioPlayer();

        virtual void Destroy();

        virtual void AssignComponents(std::shared_ptr<GameObject> self);

        virtual void Update();

        void Play(int channel = -1, int loops = 0);
        void Halt();

        float Duration() const;
        float timeRemaining() const;

        void DestroyOnFinish(bool destroy);

    private:

        /* Pointer to the audio the is played */
        std::shared_ptr<LAudio> audio;

        /* The amount of time left in the audio */
        float time_remaining = 0.0f;

        /* Whether the object should be destroyed when the audio is done playing. */
        bool terminate;
        /* Whether the audio is currently playing. */
        bool playing = false;

        /* Shared pointer to self (for self destruction) */
        std::shared_ptr<GameObject> s;
};