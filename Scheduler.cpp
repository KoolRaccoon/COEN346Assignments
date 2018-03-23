#include <stdio.h>
#include "Scheduler.hpp"
#include "Clock.hpp"
#include <iostream>
#include <fstream>
#include <condition_variable>
#include <queue>
#include <vector>
#include <string>
#include <math.h>

//#define WINDOWS
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;
mutex mu;
condition_variable cv;

Scheduler::Scheduler() {
    //priority_queue<Process*> Queue1, Queue2;
    Clk = new Clock; //creates a new clock once the scheduler is created
    Clk->StartClock(); //Starts the clock
    flagQ1Active = true; //if true, then Q1 is active, if false, Q2 is active
}

Scheduler::~Scheduler() {
    //delete output;
}
//Is used to read the processes from the input file
void Scheduler::ReadinputFile() {

    fstream input_File;
    //Acquiring the relative path of the input file
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    string current_working_dir(buff);

    //string File_Path = current_working_dir + "\\..\\input.txt";
    string File_Path = "/Users/Felix/school/University/Winter_2018/COEN346/COEN346Assignments/input.txt";
    input_File.open(File_Path);
    input_File >> Num_Process;
    
    //Storing all processes into an array
    for (int i = 0; i<Num_Process; i++) {
        string  a;
        int b, c, d;

        input_File >> a;
        ProcessArray[i].setPID(a);
        input_File >> b;
        ProcessArray[i].setaT(b);
        input_File >> c;
        ProcessArray[i].setbT(c);
        input_File >> d;
        ProcessArray[i].setPriority(d);
        ProcessArray[i].resetAllottedTimeSlots();
    }

    input_File.close();


}

//The main CPU that runs the processes from both queues
void Scheduler::runQueue(){
    Process* CurrentProcess; //Will contain the process that will get executed next
    int wait; //Used to compute the waiting time of a process
    int time; //Used to store a current time
    int runtime; //Used to store the burst time of a process
    string line; //Used for outputting log texts to the output file
    
    while (Clk->getTime() <= 1000 ){}//Waiting 1000 ms before commencing Process Execution

    while((!Queue1.empty() || !Queue2.empty()) && Clk->getTime()<10000){ //Will run until both queues are empty
        //Executing Proceses in Queue1
        while(flagQ1Active == true){ //Run until the Active flag of Queue1 is active
            if (Queue1.empty() == false){ //Run processes until the queue is empty
                CurrentProcess = Queue1.top();
                Queue1.pop(); //Removing from the queue the process that we are currently executing.

                wait = CurrentProcess->getInitialWait() + (Clk->getTime() - CurrentProcess->getPauseTime());
                CurrentProcess->CalculateQuantumTime();
                //Checking if a process's burst time remaining is less than its quantum time allotted
                if (CurrentProcess->getbT() < CurrentProcess->getQuantumTime()) {
                    runtime = CurrentProcess->getbT();
                }
                else { //If it's not then we runtime of the process is the quantum time allotted
                    runtime = CurrentProcess->getQuantumTime();
                }
                
                //If a process is already started but was paused, it resumes the process
                if(CurrentProcess->getStarted()) {
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Resumed, Granted " + to_string(runtime);
                    output->push_back(line);
                    cout << line << endl;
                    Scheduler::resumeProcess(CurrentProcess);
                }
                else { //Else the process has never been started, so it starts it
                    CurrentProcess->setStarted(true);
                    CurrentProcess->setInitialWait(Clk->getTime() - CurrentProcess->getaT());
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Started, Granted " + to_string(runtime);
                    output->push_back(line);
                    cout << line << endl;


                    //Adding the Thread for the process execution
                    thread process(&Scheduler::runProcess, this, std::ref(CurrentProcess), std::ref(Clk));
                    process.detach();
                }
                
                //Checking if a Process is terminated or not
                time = Clk->getTime();
                while((Clk->getTime() - time) < CurrentProcess->getQuantumTime()) {
                    if(CurrentProcess->getTerminated()) {
                        line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Terminated";
                        output->push_back(line);
                        cout << line << endl;
                        break;
                    }
                }
                
                //If a process is not terminated, that means it will paused instead
                if (!CurrentProcess->getTerminated()){
                    //Pausing the Process
                    Scheduler::pauseProcess(CurrentProcess);
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Paused ";
                    output->push_back(line);
                    cout << line << endl;

                    CurrentProcess->increaseAllottedTimeSlots();

                    //Updating Priority if the process ran more than twice in a row
                    if (CurrentProcess->getAllottedTimeSlots() == 2){
                        CurrentProcess->UpdatePriority(wait, Clk->getTime(), CurrentProcess->getaT(), output);
                        CurrentProcess->resetAllottedTimeSlots();
                    }
                    //Putting the process into the expired queue
                    Queue2.push(CurrentProcess);
                    
                    //Check if a process is going to run successively
                    if (!Queue1.empty()) {
                        CurrentProcess->resetAllottedTimeSlots();
                    }
                    else if(Queue2.top()->getPID() != CurrentProcess->getPID()) {
                        CurrentProcess->resetAllottedTimeSlots();
                    }
                }
            }
            else
                flagQ1Active = false;
        }
        //Executing Processes in Queue 2
        while(flagQ1Active == false){// Run until the Active flag of Queue1 is active
            if (Queue2.empty() == false){// Run processes until the queue is empty
                CurrentProcess = Queue2.top();
                Queue2.pop(); //Removing from the queue the process that we are currently executing.

                wait = CurrentProcess->getInitialWait() + (Clk->getTime() - CurrentProcess->getPauseTime());
                CurrentProcess->CalculateQuantumTime();

                if (CurrentProcess->getbT() < CurrentProcess->getQuantumTime()) {
                    runtime = CurrentProcess->getbT();
                }
                else {
                    runtime = CurrentProcess->getQuantumTime();
                }
                
                //If a process is already started but was paused, it resumes the process
                if(CurrentProcess->getStarted()) {
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Resumed, Granted " + to_string(runtime);
                    output->push_back(line);
                    cout << line << endl;
                    Scheduler::resumeProcess(CurrentProcess);
                }
                else {//Else the process has never been started, so it starts it
                    CurrentProcess->setStarted(true);
                    CurrentProcess->setInitialWait(Clk->getTime() - CurrentProcess->getaT());
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Started, Granted " + to_string(runtime);
                    output->push_back(line);
                    cout << line << endl;
                    thread process(&Scheduler::runProcess, this, std::ref(CurrentProcess), std::ref(Clk));
                    process.detach();
                }
                
                //Checking if a Process is terminated or not
                time = Clk->getTime();
                while((Clk->getTime() - time) < CurrentProcess->getQuantumTime()) {
                    if(CurrentProcess->getTerminated()) {
                        line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Terminated";
                        output->push_back(line);
                        cout << line << endl;
                        break;
                    }
                }

                //If a process is not terminated, that means it will paused instead
                if (!CurrentProcess->getTerminated()){
                    //Pausing the Process
                    Scheduler::pauseProcess(CurrentProcess);
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Paused ";
                    output->push_back(line);
                    cout << line << endl;

                    CurrentProcess->increaseAllottedTimeSlots();

                    //Updating Priority if the process ran more than twice in a row
                    if (CurrentProcess->getAllottedTimeSlots() == 2){
                        CurrentProcess->resetAllottedTimeSlots();
                        CurrentProcess->UpdatePriority(wait, Clk->getTime(), CurrentProcess->getaT(), output);
                    }
                    //Putting the process into the expired queue
                    Queue1.push(CurrentProcess);
                    
                    //Check if a process is going to run successively
                    if (!Queue2.empty()) {
                        CurrentProcess->resetAllottedTimeSlots();
                    }
                    else if(Queue1.top()->getPID() != CurrentProcess->getPID()) {
                        CurrentProcess->resetAllottedTimeSlots();
                    }
                }
            }
            else
                flagQ1Active = true;
        }
    }
    cout << "Program Terminated" << endl;
}

//Function used to keep track when a new process arrives
void Scheduler::processArrival(Clock * Clk){
    int processCount = 0;// Will be used to check what is the next process to arrive
    while (processCount < Num_Process && Clk->getTime()<10000){// Runs until all processes have arrived or until the EndOfTime
        if (ProcessArray[processCount].getaT() == Clk->getTime()){// Checks for the next Process's arrival time

            // Place arrived process into the expired queue
            if (flagQ1Active == false)
                Queue1.push(&ProcessArray[processCount]);
            else
                Queue2.push(&ProcessArray[processCount]);
            
            string line = "Time " + to_string(Clk->getTime()) + ", " + ProcessArray[processCount].getPID() + ", Arrived";
            output->push_back(line);
            cout << line << endl;
            processCount++;
        }

    }
}

void Scheduler::runProcess(Process* p, Clock *clk) {
    int time = clk->getTime();
    int runtime;

    //Calculating whether the burst time is smaller than the allotted quantum time
    if (p->getbT() < p->getQuantumTime()) {
        runtime = p->getbT();
    }
    else {
        runtime = p->getQuantumTime();
    }
    
    // Running a process for an allotted quantum time
    while (clk->getTime() < time + runtime) {
        //This is where a running process is paused by the scheduler
        while(p->getPause()){
            std::unique_lock<std::mutex> lk(mu);
            cv.wait(lk);
            lk.unlock();
        }
    }
    
    // Updating the remaining burst time recursively, this is done to have nicer quantum times
    p->setbT(p->getbT()-runtime);
    if (p->getbT() == 0){
        p->setTerminated(true);
        return;
    }
    Scheduler::runProcess(p, clk);
}

//Function used to write logs to an output file
void Scheduler::WriteOutput() {
    ofstream out("/Users/Felix/school/University/Winter_2018/COEN346/COEN346Assignments/output.txt");
    //ofstream out("output.txt");
    for(unsigned int i = 0; i < output->size(); i++){
       out << output->at(i) + " \n";
    }
    out.close();
}

//Function used to pause a thred
void Scheduler::pauseProcess(Process* p){
     lock_guard<mutex> lk(mu);
     p->setPause(true);
}

//Function used to resume a thread
void Scheduler::resumeProcess(Process* p){
     lock_guard<mutex> lk(mu);
     p->setPause(false);
     cv.notify_one();
}

void Scheduler::main(){
    ReadinputFile();
    
    //Creating a thread for the Process Arrival and QueueExecute Functions
    std::thread ProcessArrival(&Scheduler::processArrival, this, std::ref(Clk));
    std::thread QueueExecute(&Scheduler::runQueue, this);
    
    //Joining the Threads to the main thread
    ProcessArrival.join();
    QueueExecute.join();


    //Writing the Process execution log to an output file
    Scheduler::WriteOutput();
    //Scheduler::~Scheduler;
}
