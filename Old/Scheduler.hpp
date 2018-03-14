#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.hpp"
#include "Clock.hpp"
#include "MMU.hpp"
#include "Memory.hpp"
#include <thread>
#include <queue>

using namespace std;

class Scheduler
{

public:

    Scheduler();
    virtual ~Scheduler();

    void main();
    //void takeProcess (vector<Process*> &, Clock*, vector<Memory*> &, vector<MMU*>&, vector<Memory*>&);
    void takeProcess (queue<Process*> &, Clock*);
    int Num_Process;
    int Mem_Size;
    int commandCounter;
    Process ProcessArray[20];
    Memory MemoryArray[20];
    MMU MMUArray[20];
    void ReadinputFile();
    void ReadMemConfigFile();
    void ReadCommandsFile();

    Clock *Clk;

protected:

private:
};

#endif // SCHEDULER_H
