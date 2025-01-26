#pragma once

#include "TextureRenderer.hpp"

/* Texture renderer that cycles through a vector of textures in an animation. */
class AnimationRenderer : public TextureRenderer
{
    public:

        AnimationRenderer(std::shared_ptr<GameObject> obj, std::vector<std::shared_ptr<LTexture>> frames, float duration, int z = 0, bool startPlaying = true, bool renderRelative = true, bool startEnabled = true);
        ~AnimationRenderer();

        virtual void Destroy();

        virtual void Update();

        void Reset();

        void goToTime(float time);

        float Duration() const;
        void SetDuration(float duration);

        bool Playing() const;
        void Play();
        void Pause();

    private:

        /* Set of images cycled through. */
        std::vector<std::shared_ptr<LTexture>> frames;
        /* The amount of time passed between each animation frame. */
        float anim_delay, timer;
        /* The length of the animation (in seconds) */
        float len;

        /* Whether or not the animation is currently progressing. */
        bool playing;

        /* Current position in the frames vector */
        int idx = 0;
};