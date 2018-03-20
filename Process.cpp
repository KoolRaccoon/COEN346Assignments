#include "Process.hpp"
#include "Clock.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;


Process::Process() {
    Arrival_Time=0;
    Burst_Time=0;
    PID = "";
    Priority = 0;
    Started = false;
    Terminated = false;
    Pause_Time = 0;
}

Process::Process(string pid,int aT, int bT, int p){
    Arrival_Time = aT;
    Burst_Time = bT;
    PID = pid;
    Priority = p;
    Started = false;
}

void Process::run(Clock *clk) {
    int time = clk->getTime();

    int runtime;

    if (Process::getbT() < Process::getQuantumTime()) {
        runtime = Process::getbT();
    }
    else {
        runtime = Process::getQuantumTime();
    }

    if (!Process::getStarted()) {
        Process::setStarted(true);
        cout << "Time " << clk->getTime() << ", " << Process::getPID() << " Started, Granted " << runtime << endl;
    }
    else {
        cout << "Time " << clk->getTime() << ", " << Process::getPID() << " Resumed, Granted " << runtime << endl;
    }

    while (clk->getTime() < time + runtime) {}

    Process::setPauseTime(clk->getTime());

    Process::setbT(Process::getbT()-runtime); //update the burst time

    if(Process::getbT() == 0) {
        cout << "Time " << clk->getTime() << ", " << Process::getPID() << " Terminated" << endl;
        Process::setTerminated(true);
    }
    else {
        cout << "Time " << clk->getTime() << ", " << Process::getPID() << " Paused" << endl;
    }
}

void Process::UpdatePriority(int wait, int current, int arrival) {
    int bonus = ceil(10*wait/(current-arrival));
    Process::setPriority(max(100, min(Process::getPriority()-bonus+5, 139)));
    cout << "Time " << current << ", " << Process::getPID() << " priority updated to " << Process::getPriority() << endl;
}

void Process::CalculateQuantumTime() {
    if (Process::getPriority() < 100) {
        Process::setQuantumTime((140-Process::getPriority())*20);
    }
    else {
        Process::setQuantumTime((140-Process::getPriority())*5);
    }
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

void Process::setStarted(bool s) {
    Started = s;
}

void Process::setTerminated(bool t) {
    Terminated = t;
}

void Process::increaseAllottedTimeSlots(){
    AllottedTimeSlots++;
}

void Process::resetAllottedTimeSlots(){
    AllottedTimeSlots = 0;
}

void Process::setPauseTime(int pt) {
    Pause_Time = pt;
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

bool Process::getStarted(){
    return Started;
}

bool Process::getTerminated(){
    return Terminated;
}

int Process::getPauseTime() {
    return Pause_Time;
}

bool Process::operator<(const Process &p) const{
        return p.getPriority() < Process::getPriority();
}

Process::~Process() {}
