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

mutex mu1;
mutex mu2;
mutex mu3;
mutex mup;

condition_variable cv;

Scheduler::Scheduler() {
    priority_queue<Process> Q1, Q2;
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

void Scheduler::UpdatePriority(Process p, int wait, int current, int arrival) {
    int bonus = ceil(10*wait/(current-arrival));
    p.setPriority(max(100, min(p.getPriority()-bonus+5, 139)));
    cout << "Time " << current << " " << p.getPID() << ", priority updated to " << p.getPriority() << endl;
}

void Scheduler::takeProcess(priority_queue<Process> &ActiveQ, Clock * clk){
    //cout << "Enter takeProcess 1" << endl;
    bool firstProcessPicked = false;
    int time = 0;
    bool startProcess = false;
    bool startProcesschecked = false;
    bool checkifFirsttimer = false; // UNTIL THIS PART
    bool done = false;
    Process process;
    //cout << "Enter takeProcess 2" << endl;
    while (!ActiveQ.empty()) {

        process = ActiveQ.top();
        ActiveQ.pop();

        if (!firstProcessPicked) {
            checkifFirsttimer = true;
            mu1.lock();

            cout << "P" << process.getPID()<<" taken from ActiveQ, Q size is now " << ActiveQ.size() << endl;

            firstProcessPicked = true;

            mu1.unlock();

            //Checks for VERY FIRST arrival time to start executing process
            //cout << "P" << process->getPID() << "at startProcess check with bool :" << startProcess << endl;
            while (!startProcess) {
                if (clk->getTime() == process.getaT()) {
                    //cout <<process->getaT();
                    startProcess = true;
                    startProcesschecked = true;
                    time = process.getaT();
                }
            }

            if (startProcesschecked) {
                startProcess = true;
                startProcesschecked = false;
            }

            process.start(process, clk, time, done, checkifFirsttimer);
            //cout << "P" << process->getPID()<< "Checking boolean DONE :" << done << endl;
            while (!done) {}
            //cout << "P" << process->getPID() << "Exiting boolean DONE :" << done << endl;

            done = false;


        }
        else {
            //cout << "ENTERED ELSE" << endl;
            mu2.lock();

            cout << "P" << process.getPID() << " taken from ActiveQ, Q size is now " << ActiveQ.size() << endl;
            mu2.unlock();
            //cout << this_thread::get_id() << "unlocked mutex2" <<endl;

            //cout << "Time : " << clk->getTime() << ", P" << process->getPID() << " Started by CPU" << this_thread::get_id() << endl;
            //time += process->getbT();
            //while (clk->getTime() != time) {
            ////process->start(process, clk, time);
            //}
            //cout << "Process number is " << process->getPID() << endl;
            //cout << "Time : " << clk->getTime() << ", P" << process->getPID() << " Ended by CPU" << this_thread::get_id() << endl;
            done = false;
            //std::unique_lock<std::mutex> lck(mup);
            process.start(process, clk, time, done, checkifFirsttimer);
            while (!done) {}
            //cout << "Bool done is now " << dosne << endl;

        }
        //cout << "ActiveQ size is " << ActiveQ.size() << endl;
    }
    //cout << "Stuck here maybe?" << endl;
}

void Scheduler::main(){
    ReadinputFile();

    //std::thread CPU1(&Scheduler::takeProcess, this, std::ref(active), std::ref(Clk));
    //std::thread CPU2(&Scheduler::takeProcess, this, std::ref(active), std::ref(Clk));

    //std::thread CPU1(&Scheduler::takeProcess, this, std::ref(ProcessQ), std::ref(Clk), std::ref(MemoryStorage), std::ref(Commandlist), std::ref(VirtualMemory)); // Attempting to pass Clock object to thread1
    //std::thread CPU2(&Scheduler::takeProcess, this, std::ref(ProcessQ), std::ref(Clk), std::ref(MemoryStorage), std::ref(Commandlist), std::ref(VirtualMemory)); // Attempting to pass Clock object to thread2

    //cout << "waiting to join threads" << endl;
    //CPU1.join();

    //CPU2.join();

    //cout << "Memory at position 2 has value " << MemoryStorage.at(2)->getValue() << endl;
}
