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


Process::Process() {
    Arrival_Time=0;
    Burst_Time=0;
    PID = "";
    Priority = 0;
    Started = false;
    terminated = false;
}

Process::Process(string pid,int aT, int bT, int p){
    Arrival_Time = aT;
    Burst_Time = bT;
    PID = pid;
    Priority = p;
    Started = false;
}

void Process::run(Process p, Clock *clk) {
    int time = clk->getTime();

    int runtime;

    if (p.getbT() < p.getaT()) {
        runtime = p.getbT();
    }
    else {
        runtime = p.getaT();
    }

    if (!p.Started) {
        p.Started = true;
        cout << "Time " << clk->getTime() << ", " << p.getPID() << " Started, Granted " << runtime << endl;
    }
    else {
        cout << "Time " << clk->getTime() << ", " << p.getPID() << " Resumed, Granted " << runtime << endl;
    }

    while (clk->getTime() < time + runtime) {}

    p.setbT(p.getbT()-runtime); //update the burst time

    if(p.getbT() == 0) {
        cout << "Time " << clk->getTime() << ", " << p.getPID() << " Terminated" << endl;
        p.terminated = true;
    }
    else {
        cout << "Time " << clk->getTime() << ", " << p.getPID() << " Paused" << endl;
    }
}


void Process::start(Process p, Clock * clk) {
    t = new std::thread(&Process::run, this, std::ref(p), std::ref(clk));
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

void Process::increaseAllottedTimeSlots(){
    AllottedTimeSlots++;
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
