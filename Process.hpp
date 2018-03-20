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
    void run(Clock*);
    void setaT(int);
    void setbT(int);
    void setPID(string);
    void setPriority(int);
    void setQuantumTime(int);
    void setStarted(bool);
    void setPauseTime(int);
    void setTerminated(bool);
    void increaseAllottedTimeSlots();
    void resetAllottedTimeSlots();
    void UpdatePriority(int , int , int);
    void CalculateQuantumTime();

    string getPID() const;
    int getaT() const;
    int getbT() const;
    int getPriority() const;
    int getQuantumTime()const;
    int getAllottedTimeSlots();
    int getPauseTime();
    bool getStarted();
    bool getTerminated();
    bool operator<(const Process&) const;

protected:

private:
    int Arrival_Time;
    int Burst_Time;
    string PID;
    int Priority;
    int Quantum_Time;
    bool Started;
    bool Terminated;
    int AllottedTimeSlots;
    int Pause_Time;
    std::thread * t;
};

#endif // PROCESS_H
