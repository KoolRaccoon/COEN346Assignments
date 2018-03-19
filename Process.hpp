#ifndef PROCESS_H
#define PROCESS_H
#include "Clock.hpp"
#include <thread>
#include <string>
using namespace std;

class Process
{
public:
    Clock *Clk;
    Process();
    virtual ~Process();
    Process(string, int, int, int);
    void run(Process, Clock*, int&, bool&,bool&);
    void start(Process, Clock*, int&,bool&,bool&);
    void pause(Clock*, int);
    void setaT(int);
    void setbT(int);
    void setPID(string);
    void setPriority(int);
    void setQuantumTime(int);
    void increaseAllottedTimeSlots();
    void setTotalTimeRan(int);
    
    string getPID() const;
    int getaT() const;
    int getbT() const;
    int getPriority() const;
    int getQuantumTime()const;
    int getAllottedTimeSlots();
    int getTotalTimeRan();
    bool operator<(const Process&) const;

protected:

private:
    int Arrival_Time;
    int Burst_Time;
    string PID;
    int Priority;
    int Quantum_Time;
    bool Started;
    int AllottedTimeSlots;
    int TotalTimeRan;
    std::thread * t;
};

#endif // PROCESS_H
