#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.hpp"
#include "Clock.hpp"
#include <thread>
#include <queue>

using namespace std;

class Scheduler
{

public:

    Scheduler();
    virtual ~Scheduler();

    void main();
    int Num_Process;
    Process ProcessArray[20];
    void ReadinputFile();
    void runQueue();
    void processArrival(Clock*);
    Clock *Clk;

protected:

private:
    priority_queue<Process> Queue1, Queue2;
    bool flagQ1Active;
};

#endif // SCHEDULER_H
