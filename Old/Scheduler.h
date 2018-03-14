#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "Process.h"
#include "Clock.h"
#include <thread>
#include <queue>

using namespace std;

class Scheduler
{

    public:

    Scheduler();
    virtual ~Scheduler();

    void main();
    //void takeProcess (vector<Process*> &, Clock*);
    void takeProcess (queue<Process*> &, Clock*);

    //Process* tempProc;
    //Process* veryfirstProc;
    mutex mu;
    queue<Process*> active, expired;
    vector<Process*> ProcessQ;
    std::thread *CPU1;
    std::thread *CPU2;

    Clock *Clk;

    protected:

    private:
};

#endif // SCHEDULER_H
