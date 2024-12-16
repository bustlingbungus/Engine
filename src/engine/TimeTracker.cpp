#include "TimeTracker.hpp"
#include <iostream>

/* 
 * Tracks the amount of time passed between frames, and calculates framerate.
 * Initialise variables.
 * 
 * \param maxFramerate the highest FPS the time object will allow. Leave as default (-1) for uncapped framerate. 
 */
TimeTracker::TimeTracker(int maxFramerate)
: begin_time(clock()), min_dt(1000.0/maxFramerate)
{

}

TimeTracker::~TimeTracker()
{

}

/* 
 * Calculate the amount of time passed since the last frame. 
 * 
 * \warning only call this once per frame for accurate results. 
 * \note the method used to calculate delta time is only accurate to 1ms
 */
void TimeTracker::findDeltaTime()
{
    clock_t t, delta;
    // update delta time until min_dt is met
    do {
        t = clock();
        delta = t - begin_time;
    } while (delta < min_dt);
    // update begin_time for the next function call
    begin_time = t;
    // convert to seconds
    dt = delta / 1000.0f;
    // error handling for when 0ms passes between frames
    // frames per second is inverse of delta time, multiplied by 1000 (ms -> s)
    fps = (delta==0)? 1000 : 1.0/dt;
}

/* Assign the highest allowed framerate. Set to `-1` for uncapped framerate. */
void TimeTracker::setMaxFramerate(int maxFramerate)
{
    min_dt = 1000.0/maxFramerate;
}

/* The current number of frames per second. */
int TimeTracker::Framerate() const { return fps; }
/* The amount of TimeTracker elapsed since the last frame (in seconds) */
float TimeTracker::deltaTime() const { return dt; }

// initialise global object 
TimeTracker gTime = TimeTracker();