#pragma once
#include <chrono>

using TimeStamp = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::duration<float>;

class Timer
{
public:
    Timer();

    // Returns the time duration between Start() and Now, minus the paused duration.
    float GetTotalTime() const;

    // Returns the last delta time measured between Start() and Stop().
    float GetDeltaTime() const;
    float GetPausedTime() const;
    float GetStopDuration() const; // Now - StopTime

    void Reset();
    void Start();
    void Stop();
    
    float Tick();

private:
    TimeStamp m_BaseTime;
    TimeStamp m_PreviousTime;
    TimeStamp m_CurrentTime;
    
};