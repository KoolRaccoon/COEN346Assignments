#ifndef CLOCK_H
#define CLOCK_H

#include <mutex>
#include <thread>
class Clock
{
private:
    std::mutex mutex;
    int Counter;//Stores the time in miliseconds
    std::thread * t;

public:

    Clock();
    void IncrememtCounter();
    int getTime();
    void StartClock();
    ~Clock();
};

#endif // CLOCK_H
