#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "process.hpp"
#include "Clock.hpp"
#include "MMU.hpp"
#include "Memory.hpp"
#include <thread>

using namespace std;

class Scheduler
{
    
public:
    
    Scheduler();
    virtual ~Scheduler();
    
    void main();
    void takeProcess (vector<Process*> &, Clock*, vector<Memory*> &, vector<MMU*>&, vector<Memory*>&);
    int Num_Process;
    int Mem_Size;
    int commandCounter;
    Process ProcessArray[20];
    Memory MemoryArray[20];
    MMU MMUArray[20];
    void ReadinputFile();
    void ReadMemConfigFile();
    void ReadCommandsFile();
    //Process* tempProc;
    //std::thread *CPU1;
    //std::thread *CPU2;
    
    Clock *Clk;
    
protected:
    
private:
};

#endif // SCHEDULER_H
