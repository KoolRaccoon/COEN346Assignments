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
    void run(Process, Clock*);
    void start(Process, Clock*);
    void setaT(int);
    void setbT(int);
    void setPID(string);
    void setPriority(int);
    void setQuantumTime(int);
    void increaseAllottedTimeSlots();

    string getPID() const;
    int getaT() const;
    int getbT() const;
    int getPriority() const;
    int getQuantumTime()const;
    int getAllottedTimeSlots();
    bool operator<(const Process&) const;

protected:

private:
    int Arrival_Time;
    int Burst_Time;
    string PID;
    int Priority;
    int Quantum_Time;
    bool Started;
    bool terminated;
    int AllottedTimeSlots;
    std::thread * t;
};

#endif // PROCESS_H
