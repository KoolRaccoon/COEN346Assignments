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

condition_variable cv;

Scheduler::Scheduler() {
    priority_queue<Process> Queue1, Queue2;
    Clk = new Clock;
    Clk->StartClock();
    //if true, then Q1 is active, if false, Q2 is active
    flagQ1Active = true;
}

Scheduler::~Scheduler() {
    delete output;
}

void Scheduler::ReadinputFile() {

    fstream input_File;

    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    string current_working_dir(buff);

    string File_Path = current_working_dir + "\\..\\input.txt";
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
    Process CurrentProcess; //Will contain the process that will get executed next
    int wait;

    while (Clk->getTime() < 1000 ){}//Waiting 1000 ms before commencing Process Execution
    cout << "waited 1 second" << endl;
    while(!Queue1.empty() || !Queue2.empty()){//Will run until both queues are empty
        //Executing Proceses in Queue1
        while(flagQ1Active == true){
            if (Queue1.empty() == false){
                CurrentProcess = Queue1.top();
                Queue1.pop(); //Removing from the queue the process that we are currently executing.

                wait = CurrentProcess.getInitialWait() + (Clk->getTime() - CurrentProcess.getPauseTime());
                CurrentProcess.CalculateQuantumTime();
                CurrentProcess.run(Clk, output);
                CurrentProcess.increaseAllottedTimeSlots();

                if (!CurrentProcess.getTerminated()){
                    //Updating Priority if the process ran more than twice in a row
                    if (CurrentProcess.getAllottedTimeSlots() == 2){
                        CurrentProcess.UpdatePriority(wait, Clk->getTime(), CurrentProcess.getaT(), output);
                        CurrentProcess.resetAllottedTimeSlots();
                    }
                    Queue2.push(CurrentProcess);
                    //cout << "Executed Process " << CurrentProcess.getPID() << endl;

                    if (!Queue1.empty()) {
                        CurrentProcess.resetAllottedTimeSlots();
                    }
                    else if(Queue2.top().getPID() != CurrentProcess.getPID()) {
                        CurrentProcess.resetAllottedTimeSlots();
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

                wait = CurrentProcess.getInitialWait() + (Clk->getTime() - CurrentProcess.getPauseTime());
                CurrentProcess.CalculateQuantumTime();
                CurrentProcess.run(Clk, output);
                CurrentProcess.increaseAllottedTimeSlots();

                if (!CurrentProcess.getTerminated()){
                    //Updating Priority if the process ran more than twice in a row
                    if (CurrentProcess.getAllottedTimeSlots() == 2){
                        CurrentProcess.resetAllottedTimeSlots();
                        CurrentProcess.UpdatePriority(wait, Clk->getTime(), CurrentProcess.getaT(), output);
                    }
                    Queue1.push(CurrentProcess);
                    //cout << "Executed Process " << CurrentProcess.getPID() << endl;

                    if (!Queue2.empty()) {
                        CurrentProcess.resetAllottedTimeSlots();
                    }
                    else if(Queue1.top().getPID() != CurrentProcess.getPID()) {
                        CurrentProcess.resetAllottedTimeSlots();
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
    int time = Clk->getTime();
    while (processCount < Num_Process){// Runs until all processes have arrived
        if (ProcessArray[processCount].getaT() == Clk->getTime()){// Checks for the next Process's arrival time

            // Place arrived process into the expired queue
            if (flagQ1Active == false) {
                Queue1.push(ProcessArray[processCount]);
                //cout << "Added " << ProcessArray[processCount].getPID() << " to Queue1" << endl;
            }
            else {
                Queue2.push(ProcessArray[processCount]);
                //cout << "Added " << ProcessArray[processCount].getPID() << " to Queue2" << endl;
            }
            string line = "Time " + to_string(Clk->getTime()) + ", " + ProcessArray[processCount].getPID() + ", Arrived";
            output->push_back(line);
            cout << "Time " << Clk->getTime() << ", " << ProcessArray[processCount].getPID() << ", Arrived" << endl;
            processCount++;
        }

    }
}

void Scheduler::WriteOutput() {
    ofstream out("output.txt");
    for(int i = 0; i < output->size(); i++){
       out << output->at(i) + " \n";
    }
    out.close();
}

void Scheduler::main(){
    ReadinputFile();
    std::thread QueueExecute(&Scheduler::runQueue, this);
    std::thread ProcessArrival(&Scheduler::processArrival, this, std::ref(Clk));

    QueueExecute.join();
    ProcessArrival.join();
    Scheduler::WriteOutput();
    Scheduler::~Scheduler;
}
