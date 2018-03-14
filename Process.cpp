#include "Process.hpp"
#include "Clock.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <time.h>
using namespace std;

mutex mu;
mutex mub;
mutex m6;
mutex m7;
mutex m8;
mutex m9;
mutex m5;
mutex m10;


Process::Process() {
    Arrival_Time=0;
    Burst_Time=0;
    PID = "";
    Priority = 0;
}

Process::Process(string pid,int aT, int bT, int p){
    Arrival_Time = aT;
    Burst_Time = bT;
    PID = pid;
    Priority = p;
}

void Process::run(Process tempProc, Clock *clk, int &timer, bool& done,bool& checkifFirsttimer) {}

void Process::start(Process tempProc, Clock * clk, int &timer, bool& done, bool& checkifFirsttimer) {
    t = new std::thread(&Process::run, this, std::ref(tempProc), std::ref(clk), std::ref(timer), std::ref(done), std::ref(checkifFirsttimer));
}

void Process::setaT(int aT){
    Arrival_Time = aT;
}

void Process::setbT(int bT){
    Burst_Time = bT;
}

void Process::setPID(string pid){
    PID = pid;
}

void Process::setPriority(int p){
    Priority = p;
}

void Process::setQuantumTime(int qT) {
    Quantum_Time = qT;
}

int Process::getaT() const{
    return Arrival_Time;
}

int Process::getbT() const{
    return Burst_Time;
}

string Process::getPID() const{
    return PID;
}

int Process::getPriority() const{
    return Priority;
}

int Process::getQuantumTime() const{
    return Quantum_Time;
}

bool Process::operator<(const Process &p) const{
        return Priority < p.getPriority();
}

Process::~Process() {}
