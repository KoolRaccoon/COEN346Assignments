#include <stdio.h>
#include "Scheduler.hpp"
#include "Clock.hpp"
#include <iostream>
#include <fstream>
#include <condition_variable>
#include <queue>
#include <vector>
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

Scheduler::~Scheduler() {}

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
    }

    input_File.close();


}

void Scheduler::CalculateQuantumTime(Process p) {
    if (p.getPriority() < 100) {
        p.setQuantumTime((140-p.getPriority())*20);
    }
    else {
        p.setQuantumTime((140-p.getPriority())*5);
    }

}

Process Scheduler::UpdatePriority(Process p, int wait, int current, int arrival) {
    int bonus = ceil(10*wait/(current-arrival));
    p.setPriority(max(100, min(p.getPriority()-bonus+5, 139)));
    cout << "Time " << current << " " << p.getPID() << ", priority updated to " << p.getPriority() << endl;
    return p;
}


void Scheduler::runQueue(){
    Process CurrentProcess; //Will contain the process that will get executed next

    while(Queue1.empty() == false && Queue2.empty() == false){//Will run until both queues are empty
        //Executing Proceses in Queue1
        while(flagQ1Active == true){
            if (Queue1.empty() == false){
                CurrentProcess = Queue1.top();
                Queue1.pop(); //Removing from the queue the process that we are currently executing.
                Scheduler::CalculateQuantumTime(CurrentProcess);
                //CurrentProcess.run();
                CurrentProcess.increaseAllottedTimeSlots();
                //Updating Priority if the process ran more than twice in a row
                if (CurrentProcess.getAllottedTimeSlots()>0 && CurrentProcess.getAllottedTimeSlots()%2 ==0){
                    //CurrentProcess = Scheduler::UpdatePriority(CurrentProcess, int wait, Clk->getTime(), CurrentProcess.getaT())
                }
                CurrentProcess.setbT(CurrentProcess.getbT()-CurrentProcess.getQuantumTime()); //Updating the burst time.
                Queue2.push(CurrentProcess);
            }
            else
                flagQ1Active = false;
        }
        //Executing Processes in Queue 2
        while(flagQ1Active == false){
            if (Queue2.empty() == false){
                CurrentProcess = Queue2.top();
                Queue2.pop(); //Removing from the queue the process that we are currently executing.
                Scheduler::CalculateQuantumTime(CurrentProcess);
                //CurrentProcess.run();
                CurrentProcess.increaseAllottedTimeSlots();
                //Updating Priority if the process ran more than twice in a row
                if (CurrentProcess.getAllottedTimeSlots()>0 && CurrentProcess.getAllottedTimeSlots()%2 ==0){
                    //CurrentProcess = Scheduler::UpdatePriority(CurrentProcess, int wait, Clk->getTime(), CurrentProcess.getaT())
                }
                CurrentProcess.setbT(CurrentProcess.getbT()-CurrentProcess.getQuantumTime()); //Updating the burst time.
                Queue2.push(CurrentProcess);
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
                Queue1.push(ProcessArray[processCount]);
                cout << "Added " << ProcessArray[processCount].getPID() << " to Queue1" << endl;
            }
            else {
                Queue2.push(ProcessArray[processCount]);
                cout << "Added " << ProcessArray[processCount].getPID() << " to Queue2" << endl;
            }
            cout << "Time " << Clk->getTime() << ", " << ProcessArray[processCount].getPID() << ", Arrived" << endl;
            processCount++;
        }

    }
}

void Scheduler::main(){
    ReadinputFile();

    std::thread ProcessArrival(&Scheduler::processArrival, this, std::ref(Clk));
    std::thread Queue1();

    ProcessArrival.join();

    /*

     Either we used two threads to run each QUEUE and we add another one to check when a process arrives

     */


    //std::thread CPU1(&Scheduler::takeProcess, this, std::ref(active), std::ref(Clk));
    //std::thread CPU2(&Scheduler::takeProcess, this, std::ref(active), std::ref(Clk));

    //std::thread CPU1(&Scheduler::takeProcess, this, std::ref(ProcessQ), std::ref(Clk), std::ref(MemoryStorage), std::ref(Commandlist), std::ref(VirtualMemory)); // Attempting to pass Clock object to thread1
    //std::thread CPU2(&Scheduler::takeProcess, this, std::ref(ProcessQ), std::ref(Clk), std::ref(MemoryStorage), std::ref(Commandlist), std::ref(VirtualMemory)); // Attempting to pass Clock object to thread2

    //cout << "waiting to join threads" << endl;
    //CPU1.join();

    //CPU2.join();

    //cout << "Memory at position 2 has value " << MemoryStorage.at(2)->getValue() << endl;
}
