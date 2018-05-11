#ifndef TIMER_H
#define TIMER_H
class Timer
{
    using hrClock = std::chrono::high_resolution_clock;
    using durationDouble = std::chrono::duration<double>;
    std::chrono::time_point<hrClock> start;
public:
    Timer() : start{ hrClock::now() } {}
    void reset() {start = hrClock::now();}
    double elapsed() const {return durationDouble (hrClock::now() - start).count();}
};
#endif
