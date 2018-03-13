#ifndef CLOCK_H
#define CLOCK_H

#include <mutex>
#include <thread>
class Clock
{
private:
    std::mutex mutex;
    int Counter;
    std::thread * t;
    
public:
    
    Clock();
    //Clock( const Clock& obj);
    void IncrememtCounter();
    int getTime();
    void StartClock();
    ~Clock();
};

#endif // CLOCK_H
