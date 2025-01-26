#include "AnimationRenderer.hpp"
#include "../../TimeTracker.hpp"

/*
 * Texture renderer that cycles through a vector of textures in an animation.
 *
 * \note Animation images will be cycled such that an equal amount of time is passed 
 * between each animation frame.
 * 
 * \param obj The game object the component is attached to 
 * \param frames A vector of the images to cycle through.
 * \param duration The duration of a full animation cycle (in seconds). 
 * \param z Value used to determine which order objects are rendered in.
 * \param renderRelative Whether or not the object is rendered relative to cameras, or on the window directly.
 * \param startEnabled Whether or not the component is active on creation. `true` by default.
 */
AnimationRenderer::AnimationRenderer(std::shared_ptr<GameObject> obj, std::vector<std::shared_ptr<LTexture>> frames, float duration, int z, bool startPlaying, bool renderRelative, bool startEnabled)
: TextureRenderer(obj, nullptr, z, renderRelative, startEnabled),
  frames(frames), len(duration), playing(startPlaying)
{
    // find the time to elapse between each animation frame
    timer=anim_delay = len/float(frames.size()-1);
}

AnimationRenderer::~AnimationRenderer()
{
    Destroy();
}

void AnimationRenderer::Destroy()
{
    TextureRenderer::Destroy();
    frames.clear();
}

/*
 * Loops through animation frames when a time interval is surpassed, as long as the
 * animation is currently playing.
 * 
 * Also performs regular TextureRenderer update behaviour. 
 */
void AnimationRenderer::Update()
{
    // update normally first, then decrement timer
    TextureRenderer::Update();
    if (playing) timer -= gTime.deltaTime();

    // proceed to next frame, and reset timer
    if (timer <= 0.0f) {
        // when the end of the vector is reached, go back to beginning
        if (++idx>=frames.size()) idx = 0;
        timer = anim_delay;
    }

    // set texture from vector
    texture = frames[idx];
}

/* Resets the animation from beginning. */
void AnimationRenderer::Reset() {
    idx = 0;
    timer = anim_delay;
}

/*
 * Sets the animation frame to be at a certain time in the animation's cycle.
 * 
 * \warning Specifying a negative time will not do anything.
 * 
 * \note Specifying a time greater than the animation's duration will loop into
 * subsequent cycles of the animation (it will set to `time % duration`, basically).
 */
void AnimationRenderer::goToTime(float time)
{
    if (time<0.0f) return;
    // bootleg modulo
    if (time>len) time -= int(time/len)*len;
    // get time as interpolator and go to corresponding index
    idx = int((time/len)*frames.size());
    // find remainder for timer
    timer = time - (anim_delay*(float)idx);
}

/* The length of the animation (in seconds) */
float AnimationRenderer::Duration() const { return len; }
/* Set the length of the animation (in seconds) */
void AnimationRenderer::SetDuration(float duration) { 
    len = duration;
    timer=anim_delay = len/float(frames.size()-1);
    idx = 0;
}

/* Whether or not the animation is currently progressing. */
bool AnimationRenderer::Playing() const { return playing; }
/* Make the animation progress. */
void AnimationRenderer::Play() { playing = true; }
/* Make the animation stop progressing. */
void AnimationRenderer::Pause() { playing = false; }