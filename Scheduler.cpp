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
    priority_queue<Process*> Queue1, Queue2;
    Clk = new Clock;
    Clk->StartClock();
    //if true, then Q1 is active, if false, Q2 is active
    flagQ1Active = true;
}

Scheduler::~Scheduler() {
    //delete output;
}

void Scheduler::ReadinputFile() {

    fstream input_File;

    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    string current_working_dir(buff);

    string File_Path = current_working_dir + "\\..\\input.txt";
    //string File_Path = "/Users/Felix/school/University/Winter_2018/COEN346/COEN346Assignments/input.txt";
    input_File.open(File_Path);
    input_File >> Num_Process;
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

void Scheduler::runQueue(){
    Process* CurrentProcess; //Will contain the process that will get executed next
    int wait;
    int time;
    int runtime;
    string line;

    while (Clk->getTime() <= 1000 ){}//Waiting 1000 ms before commencing Process Execution
    //cout << "waited 1 second" << endl;
    while(!Queue1.empty() || !Queue2.empty()){//Will run until both queues are empty
        //Executing Proceses in Queue1
        while(flagQ1Active == true){
            if (Queue1.empty() == false){
                CurrentProcess = Queue1.top();
                Queue1.pop(); //Removing from the queue the process that we are currently executing.

                wait = CurrentProcess->getInitialWait() + (Clk->getTime() - CurrentProcess->getPauseTime());
                CurrentProcess->CalculateQuantumTime();

                if (CurrentProcess->getbT() < CurrentProcess->getQuantumTime()) {
                    runtime = CurrentProcess->getbT();
                }
                else {
                    runtime = CurrentProcess->getQuantumTime();
                }

                if(CurrentProcess->getStarted()) {
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Resumed, Granted " + to_string(runtime);
                    output->push_back(line);
                    cout << line << endl;
                    Scheduler::resumeProcess(CurrentProcess);
                }
                else {
                    CurrentProcess->setStarted(true);
                    CurrentProcess->setInitialWait(Clk->getTime() - CurrentProcess->getaT());
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Started, Granted " + to_string(runtime);
                    output->push_back(line);
                    cout << line << endl;


                    //CurrentProcess.start(Clk);
                    thread process(&Scheduler::runProcess, this, std::ref(CurrentProcess), std::ref(Clk));
                    process.detach();
                }

                time = Clk->getTime();
                while((Clk->getTime() - time) < CurrentProcess->getQuantumTime()) {
                    if(CurrentProcess->getTerminated()) {
                        line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Terminated";
                        output->push_back(line);
                        cout << line << endl;
                        break;
                    }
                }

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

                    Queue2.push(CurrentProcess);
                    //cout << "Executed Process " << CurrentProcess.getPID() << endl;

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
        while(flagQ1Active == false){
            if (Queue2.empty() == false){
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

                if(CurrentProcess->getStarted()) {
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Resumed, Granted " + to_string(runtime);
                    output->push_back(line);
                    cout << line << endl;
                    Scheduler::resumeProcess(CurrentProcess);
                }
                else {
                    CurrentProcess->setStarted(true);
                    CurrentProcess->setInitialWait(Clk->getTime() - CurrentProcess->getaT());
                    line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Started, Granted " + to_string(runtime);
                    output->push_back(line);
                    cout << line << endl;
                    //CurrentProcess.start(Clk);
                    thread process(&Scheduler::runProcess, this, std::ref(CurrentProcess), std::ref(Clk));
                    process.detach();
                }
                //cout << "Process start was called" << endl;
                time = Clk->getTime();
                while((Clk->getTime() - time) < CurrentProcess->getQuantumTime()) {
                    if(CurrentProcess->getTerminated()) {
                        line = "Time " + to_string(Clk->getTime()) + ", " + CurrentProcess->getPID() + " Terminated";
                        output->push_back(line);
                        cout << line << endl;
                        break;
                    }
                }


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
                    Queue1.push(CurrentProcess);
                    //cout << "Executed Process " << CurrentProcess.getPID() << endl;

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

void Scheduler::processArrival(Clock * Clk){
    int processCount = 0;// Will be used to check what is the next process to arrive
    while (processCount < Num_Process){// Runs until all processes have arrived
        if (ProcessArray[processCount].getaT() == Clk->getTime()){// Checks for the next Process's arrival time

            // Place arrived process into the expired queue
            if (flagQ1Active == false) {
                Queue1.push(&ProcessArray[processCount]);
                //cout << "Added " << ProcessArray[processCount].getPID() << " to Queue1" << endl;
            }
            else {
                Queue2.push(&ProcessArray[processCount]);
                //cout << "Added " << ProcessArray[processCount].getPID() << " to Queue2" << endl;
            }
            string line = "Time " + to_string(Clk->getTime()) + ", " + ProcessArray[processCount].getPID() + ", Arrived";
            output->push_back(line);
            cout << "Time " << Clk->getTime() << ", " << ProcessArray[processCount].getPID() << ", Arrived" << endl;
            processCount++;
        }

    }
}

void Scheduler::runProcess(Process* p, Clock *clk) {
    int time = clk->getTime();
    int runtime;

    if (p->getbT() < p->getQuantumTime()) {
        runtime = p->getbT();
    }
    else {
        runtime = p->getQuantumTime();
    }

    //while (clk->getTime() < time + runtime) {
    /*while(p->getbT() > 0){
        while(p->getPause()){
            std::unique_lock<std::mutex> lk(mu);
            cv.wait(lk);
            lk.unlock();
        }
        //while(clk->getTime()-time == 0) {}
        //time = clk->getTime();
        this_thread::sleep_for(chrono::milliseconds(1));
        p->setbT(p->getbT()-1);
        //cout << "Process ran for 1ms" << endl;
    }*/
    while (clk->getTime() < time + runtime) {
        while(p->getPause()){
            std::unique_lock<std::mutex> lk(mu);
            cv.wait(lk);
            lk.unlock();
        }
        //cout << "Process ran for 1ms" << endl;
    }

    p->setbT(p->getbT()-runtime);
    if (p->getbT() == 0){
        p->setTerminated(true);
        return;
    }
    Scheduler::runProcess(p, clk);
}

void Scheduler::WriteOutput() {
    //ofstream out("/Users/Felix/school/University/Winter_2018/COEN346/COEN346Assignments/output.txt");
    ofstream out("output.txt");
    for(unsigned int i = 0; i < output->size(); i++){
       out << output->at(i) + " \n";
    }
    out.close();
}

void Scheduler::pauseProcess(Process* p){
     lock_guard<mutex> lk(mu);
     p->setPause(true);
}

void Scheduler::resumeProcess(Process* p){
     lock_guard<mutex> lk(mu);
     p->setPause(false);
     cv.notify_one();
}

void Scheduler::main(){
    ReadinputFile();
    std::thread QueueExecute(&Scheduler::runQueue, this);
    std::thread ProcessArrival(&Scheduler::processArrival, this, std::ref(Clk));

    QueueExecute.join();
    ProcessArrival.join();
    Scheduler::WriteOutput();
    //Scheduler::~Scheduler;
}
