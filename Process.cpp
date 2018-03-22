#include "Process.hpp"
#include "Clock.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <condition_variable>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;

mutex *mu = new mutex;
condition_variable cv;

Process::Process() {
    Arrival_Time=0;
    Burst_Time=0;
    PID = "";
    Priority = 0;
    Started = false;
    Terminated = false;
    Pause_Time = 0;
    Initial_Wait = 0;
    Pause = true;
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
    string line;
    int runtime;

    if (Process::getbT() < Process::getQuantumTime()) {
        runtime = Process::getbT();
    }
    else {
        runtime = Process::getQuantumTime();
    }

    //while (clk->getTime() < time + runtime) {}
    while(Process::getbT() > 0){
        if (!Process::getPause()) {
            Process::setbT(Process::getbT()-1);
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }


    Process::setPauseTime(clk->getTime());

    Process::setbT(Process::getbT()-runtime); //update the burst time

    if(Process::getbT() == 0) {
        cout << "Time " << clk->getTime() << ", " << Process::getPID() << " Terminated" << endl;
        Process::setTerminated(true);
    }
    else {
        line = "Time " + to_string(clk->getTime()) + ", " + Process::getPID() + " Paused";
        output->push_back(line);
        cout << "Time " << clk->getTime() << ", " << Process::getPID() << " Paused" << endl;
    }
}

void Process::UpdatePriority(int wait, int current, int arrival, vector<string> *output) {
    int bonus = ceil(10*wait/(current-arrival));
    Process::setPriority(max(100, min(Process::getPriority()-bonus+5, 139)));
    string line = "Time " + to_string(current) + ", " + Process::getPID() + " priority updated to " + to_string(Process::getPriority());
    output->push_back(line);
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

void Process::start(Clock *clk) {
    thread process(Process::run, this, std::ref(clk));
    process.join();
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

void Process::setPause(bool p) {
    Pause = p;
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

void Process::setInitialWait(int iw) {
    Initial_Wait = iw;
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

int Process::getInitialWait() {
    return Initial_Wait;
}

bool Process::getPause() {
    return Pause;
}

void Process::pauseProcess(){
     //pause
     lock_guard<mutex> lk(mu);
     Process:setPause=true;
}
void Process::resumeProcess(){
     lock_guard<mutex> lk(mu);
     Process::setPause=false;
     cv.notify_one();
     //resume t2
}

bool Process::operator<(const Process &p) const{
        return p.getPriority() < Process::getPriority();
}

Process::~Process() {}
