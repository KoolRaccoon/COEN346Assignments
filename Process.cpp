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
    Started = false;
}

Process::Process(string pid,int aT, int bT, int p){
    Arrival_Time = aT;
    Burst_Time = bT;
    PID = pid;
    Priority = p;
    Started = false;
}

void Process::run(Process p, Clock *clk, int &timer, bool& done,bool& checkifFirsttimer) {
    int b = p.getbT();
    m5.lock();
    m5.unlock();
    timer += p.getbT();
    srand(time(NULL));

    if (!p.Started) {
        p.Started = true;
        cout << "Time " << clk->getTime() << ", " << p.getPID() << " Started, Granted " << p.getaT() << endl;
    }
    else {
        cout << "Time " << clk->getTime() << ", " << p.getPID() << " Resumed, Granted " << p.getaT() << endl;
    }
        int a = 0;
        int c = 0;
        int sumOa = 0;
        int d = 0;
        int APICall = 0;
        int valuetostore = 0;
        int varID = 0;

        //cout << "ITS FIRST TIME" << endl;
        while(clk->getTime() < timer && b!=0){
            while(b>0){
                a = rand() % 200;
                sumOa += a;
                d = clk->getTime();
                c = b - a;
                //cout << "P" << p->getPID() << "'s C: " << c << ", B: " << b << ", A: " << a << endl;
                //if (b > 0) {

                        //std::this_thread::sleep_for(std::chrono::milliseconds(a));
                        b -= a;
                        while (clk->getTime() != (d + a)) {}
                        m6.lock();
                        //cout << "P" << p->getPID() << " Finished command at " << clk->getTime() << endl;
                        m6.unlock();
                        //cout << "P" << p->getPID() << " Burst time left is " <<b<< endl;
                        m7.lock();
                        //cout << "P" << p->getPID() << " Not enough time to execute next command, sleeping for the rest of burst time. " << endl;
                        m7.unlock();
                        //std::this_thread::sleep_for(std::chrono::milliseconds(b));
                        b -= b;
                        //cout << b << endl;
                }
            }
        done = true;
        checkifFirsttimer = false;
}


void Process::start(Process p, Clock * clk, int &timer, bool& done, bool& checkifFirsttimer) {
    t = new std::thread(&Process::run, this, std::ref(p), std::ref(clk), std::ref(timer), std::ref(done), std::ref(checkifFirsttimer));
}

void Process::pause(Clock *clk, int waitTime) {
    while(clk->getTime() < waitTime) {}
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

void Process::setAllottedTimeSlots(int TimeSlots){
    AllottedTimeSlots +=TimeSlots;
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

int Process::getAllottedTimeSlots(){
    return AllottedTimeSlots;
}
bool Process::operator<(const Process &p) const{
        return Priority < p.getPriority();
}

Process::~Process() {}
