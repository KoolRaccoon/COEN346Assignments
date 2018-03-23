#include "Process.hpp"
#include "Clock.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <time.h>
#include <math.h>

using namespace std;

//Initializing a process when the object is first created 
Process::Process() {
    Arrival_Time=0;
    Burst_Time=0;
    PID = "";
    Priority = 0;
    Started = false;
    Terminated = false;
    Pause_Time = 0;
    Initial_Wait = 0;
    Pause = false;
}

//Used to calculate the priority of a process when it needs to be updated
void Process::UpdatePriority(int wait, int current, int arrival, vector<string> *output) {
    
    //The priority is calculated based on the formulas provided
    int bonus = ceil(10*wait/(current-arrival));
    Process::setPriority(max(100, min(Process::getPriority()-bonus+5, 139)));
    string line = "Time " + to_string(current) + ", " + Process::getPID() + " priority updated to " + to_string(Process::getPriority());
    output->push_back(line);
    cout << "Time " << current << ", " << Process::getPID() << " priority updated to " << Process::getPriority() << endl;
}

//Used to calculate the quantum time of a process
void Process::CalculateQuantumTime() {
    
    //Quantum time is calculated using the formulas provided 
    if (Process::getPriority() < 100) {
        Process::setQuantumTime((140-Process::getPriority())*20);
    }
    else {
        Process::setQuantumTime((140-Process::getPriority())*5);
    }
}

//Sets the arrival of a process at its creation
void Process::setaT(int aT){
    Arrival_Time = aT;
}

//Sets the burst time of a process at its creation
void Process::setbT(int bT){
    Burst_Time = bT;
}

//Sets the PID of a process when it gets created
void Process::setPID(string pid){
    PID = pid;
}

//Sets a priority of a process when it is updated
void Process::setPriority(int p){
    Priority = p;
}

//Sets the quantum of process
void Process::setQuantumTime(int qT) {
    Quantum_Time = qT;
}

//Sets if a process has already been started
void Process::setStarted(bool s) {
    Started = s;
}

//Sets if a process has terminated
void Process::setTerminated(bool t) {
    Terminated = t;
}

//Sets the pause value of a process
void Process::setPause(bool p) {
    Pause = p;
}

//If ia process has ran for more than once consecutively it will increases it's AllottedTime Slots
void Process::increaseAllottedTimeSlots(){
    AllottedTimeSlots++;
}

//Resets if a process has not ran consecutively
void Process::resetAllottedTimeSlots(){
    AllottedTimeSlots = 0;
}

//Set for how long a process has been paused
void Process::setPauseTime(int pt) {
    Pause_Time = pt;
}

//Set how long the process has waited between when it arrived and when it first executed
void Process::setInitialWait(int iw) {
    Initial_Wait = iw;
}

//Returns the process's arrival time
int Process::getaT() const{
    return Arrival_Time;
}

//Returns the procces's burst time
int Process::getbT() const{
    return Burst_Time;
}

//Returns the process's PID
string Process::getPID() const{
    return PID;
}

//Returns the process's priority
int Process::getPriority() const{
    return Priority;
}

//Returns for how long a process gets to run in a timeslote
int Process::getQuantumTime() const{
    return Quantum_Time;
}

//Returns how many times a process has ran consecutively
int Process::getAllottedTimeSlots(){
    return AllottedTimeSlots;
}

bool Process::getStarted(){
    return Started;
}

//Returns if the process is terminated
bool Process::getTerminated(){
    return Terminated;
}

//Returns for how long the process has paused
int Process::getPauseTime() {
    return Pause_Time;
}

//Returns the time between the process arrives and when it first runs
int Process::getInitialWait() {
    return Initial_Wait;
}

//Returns if a process is paused
bool Process::getPause() {
    return Pause;
}

//Used to store processes in the priority_queue based on the priority value of the process
bool Process::operator<(const Process &p) const{
        return p.getPriority() < Process::getPriority();
}

Process::~Process() {}
