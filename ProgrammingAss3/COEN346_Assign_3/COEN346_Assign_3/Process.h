#ifndef PROCESS_H
#define PROCESS_H

#include <vector>

using namespace std;

class Process
{
    public:
        Process();
        virtual ~Process();
        Process(int, int, int);
        void setaT(int);
        void setbT(int);
        void setPID(int);
        int getPID();
        int getaT();
        int getbT();

    protected:

    private:
        int Arrival_Time;
        int Burst_Time;
        int PID;
};

#endif // PROCESS_H
