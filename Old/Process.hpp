#ifndef PROCESS_H
#define PROCESS_H
#include "Clock.hpp"
#include "Memory.hpp"
#include "MMU.hpp"
#include <vector>
#include <thread>

using namespace std;

class Process
{
public:
    Clock *Clk;
    Process();
    virtual ~Process();
    Process(int, int, int);
    void run(Process*, Clock*, int&, bool&,bool&);
    //void start(Process*, Clock*, int&,bool&,bool&, vector<Memory*>&, vector<MMU*>&, vector<Memory*>&);
    void start(Process*, Clock*, int&,bool&,bool&);
    void setaT(int);
    void setbT(int);
    void setPID(int);
    //void store(vector<Memory*>&,vector<Memory*>&,int&, int&);
    //void release(vector<Memory*>&, int);
    int getPID();
    int getaT();
    int getbT();

protected:

private:
    int Arrival_Time;
    int Burst_Time;
    int PID;
    std::thread * t;
};

#endif // PROCESS_H
