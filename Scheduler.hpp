#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.hpp"
#include "Clock.hpp"
#include <thread>
#include <queue>
#include <vector>

using namespace std;


class Scheduler
{

public:

    Scheduler();
    virtual ~Scheduler();

    void main();
    int Num_Process;
    void ReadinputFile();
    void runQueue();
    void processArrival(Clock*);
    void runProcess(Process*, Clock*);
    void WriteOutput();
    void resumeProcess(Process*);
    void pauseProcess(Process*);
    Clock *Clk;

protected:

private:
    priority_queue<Process*> Queue1, Queue2; // Two priority queues to run processes
    vector<string> *output = new vector<string>;
    bool flagQ1Active;//Used to check which queue is expired
    Process* ProcessArray = new Process[20];
};

#endif // SCHEDULER_H
