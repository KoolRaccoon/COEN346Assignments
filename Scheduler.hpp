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
    Process ProcessArray[20];
    void ReadinputFile();
    void runQueue();
    void processArrival(Clock*);
    void runProcess(Process, Clock*);
    void WriteOutput();
    Clock *Clk;

protected:

private:
    priority_queue<Process> Queue1, Queue2;
    vector<string> *output = new vector<string>;
    bool flagQ1Active;
    //condition_variable cv;

};

#endif // SCHEDULER_H
