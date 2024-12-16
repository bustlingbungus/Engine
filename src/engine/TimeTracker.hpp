#pragma once

#include <ctime>

/* Tracks the amount of time passed between frames, and calculates framerate. */
class TimeTracker
{
    public:

        TimeTracker(int maxFramerate = -1);
        ~TimeTracker();

        void findDeltaTime();
        void setMaxFramerate(int maxFramerate);

        int Framerate() const;
        float deltaTime() const;

    private:

        /* Variable used for calculating delta time. */
        clock_t begin_time;
        /* The minimum amount of time that may pass between frames. */
        clock_t min_dt;
        /* The amount of time passed since the last frame (in seconds). */
        float dt = 0;
        /* The current number of frames per second. */
        int fps = 0;
};

/* Global TimeTracker object */
extern TimeTracker gTime;