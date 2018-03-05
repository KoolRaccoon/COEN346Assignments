#include "Process.h"
#include "Clock.h"
#include <iostream>

using namespace std;


Process::Process()
{
    Arrival_Time=0;
    Burst_Time=0;
    PID = 0;
}

Process::Process(int pid,int aT, int bT){
    Arrival_Time = aT;
    Burst_Time = bT;
    PID = pid;
}

void Process::setaT(int aT){
    Arrival_Time = aT;
}

void Process::setbT(int bT){
    Burst_Time = bT;
}

void Process::setPID(int pid){
    PID = pid;
}

int Process::getaT(){
    return Arrival_Time;
}

int Process::getbT(){
    return Burst_Time;
}

int Process::getPID(){
    return PID;
}

Process::~Process()
{
    //dtor
}
