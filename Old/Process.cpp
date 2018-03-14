#include "Process.hpp"
#include "Clock.hpp"
#include "Memory.hpp"
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

void print(vector<Memory*>&);

Process::Process() {
    Arrival_Time=0;
    Burst_Time=0;
    PID = 0;
}

Process::Process(int pid,int aT, int bT){
    Arrival_Time = aT;
    Burst_Time = bT;
    PID = pid;
}

/*void Process::run(Process *tempProc, Clock *clk, int &timer, bool& done,bool& checkifFirsttimer, vector<Memory*>& MemoryStorage, vector<MMU*>&Commandlist, vector<Memory*>&VirtualMemory) {
    //cout << "First Variable ID is " << MemoryStorage.at(0)->getVarID() << endl;
    //cout << "First command action is: " << Commandlist.at(0)->getActionID() << endl;
    //cout << tempProc->getPID() << "Entered run()" << endl;
    //cout << tempProc->getPID() << endl;
    //cout << timer << endl;
    int b = tempProc->getbT();
    //cout << "Burst time is " << b << endl;
    m5.lock();
    cout << "Time " << clk->getTime() << ", P" << tempProc->getPID() << " Started" << endl;
    m5.unlock();
    timer += tempProc->getbT();
    srand(time(NULL));

    if (checkifFirsttimer) {
        int a = 0;
        int c = 0;
        int sumOa = 0;
        int d = 0;
        int commandcounter=0;
        int commandsize = Commandlist.size() - 1;
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
                //cout << "P" << tempProc->getPID() << "'s C: " << c << ", B: " << b << ", A: " << a << endl;
                //if (b > 0) {
                //cout << "P" << tempProc->getPID() << "'s total of BT executed "<< sumOa << endl;
                if (commandcounter < commandsize) {

                    if (c > 0) {
                        string action = Commandlist.at(commandcounter)->getAction();
                        if (action == "Store") {
                            m10.lock();
                            cout << "Time " << clk->getTime() << ", P" << tempProc->getPID() << ", " << Commandlist.at(commandcounter)->getAction() << ": Variable " << Commandlist.at(commandcounter)->getVarID() << ", Value " << Commandlist.at(commandcounter)->getToValue() << endl;
                            m10.unlock();
                        }
                        else {
                            m10.lock();
                            cout << "Time " << clk->getTime() << ", P" << tempProc->getPID() << ", " << Commandlist.at(commandcounter)->getAction() << ": Variable " << Commandlist.at(commandcounter)->getVarID() << endl;
                            m10.unlock();
                        }
                        APICall = Commandlist.at(commandcounter)->getActionID();
                        //cout << APICall << endl;
                        if (APICall == 1) {
                            valuetostore = Commandlist.at(commandcounter)->getToValue();
                            varID = Commandlist.at(commandcounter)->getVarID();
                        }
                        else {
                            varID = Commandlist.at(commandcounter)->getVarID();
                        }
                        commandcounter++;
                        switch ( APICall )
                        {
                            case 1:
                                store(MemoryStorage,VirtualMemory, varID, valuetostore);
                                //cout << MemoryStorage.at(2)->getValue() << endl;
                                break;
                            case 2:
                                release(MemoryStorage, varID);
                                //cout << MemoryStorage.at(varPos)->getValue() << endl;
                                break;
                        }
                        //std::this_thread::sleep_for(std::chrono::milliseconds(a));
                        b -= a;
                        while (clk->getTime() != (d + a)) {} //////
                        m6.lock();
                        //cout << "P" << tempProc->getPID() << " Finished command at " << clk->getTime() << endl;
                        m6.unlock();
                    }
                    else {
                        //cout << "P" << tempProc->getPID() << " Burst time left is " <<b<< endl;
                        m7.lock();
                        //cout << "P" << tempProc->getPID() << " Not enough time to execute next command, sleeping for the rest of burst time. " << endl;
                        m7.unlock();
                        //std::this_thread::sleep_for(std::chrono::milliseconds(b));
                        b -= b;
                        //cout << b << endl;
                    }
                }
                else {

                    if (c > 0) {
                        //std::this_thread::sleep_for(std::chrono::milliseconds(a));
                        b -= a;
                        while (clk->getTime() != (d + a)) {} //////
                        m6.lock();
                        //cout << "P" << tempProc->getPID() << " Finished command at " << clk->getTime() << endl;
                        m6.unlock();
                    }
                    else {
                        //cout << "P" << tempProc->getPID() << " Burst time left is " <<b<< endl;
                        m7.lock();
                        //cout << "P" << tempProc->getPID() << " Not enough time to execute next command, sleeping for the rest of burst time. " << endl;
                        m7.unlock();
                        //std::this_thread::sleep_for(std::chrono::milliseconds(b));
                        b -= b;
                        //cout << b << endl;
                    }
                }
            }
        }
        done = true;
        checkifFirsttimer = false;
    }
    else if (!checkifFirsttimer) {
        int a = 0;
        int c = 0;
        int sumOa = 0;
        int d = 0;
        int commandcounter = 0;
        int commandsize = Commandlist.size() - 1;
        int APICall = 0;
        int valuetostore = 0;
        int varID = 0;
        while (clk->getTime() != timer) {
            while(b>0){
                a = rand() % 200;
                sumOa += a;
                d = clk->getTime();
                c = b - a;
                //		if (b > 0) {
                if (commandcounter < commandsize) {
                    if (c > 0) {
                        string action = Commandlist.at(commandcounter)->getAction();
                        if (action == "Store") {
                            m10.lock();
                            cout << "Time " << clk->getTime() << ", P" << tempProc->getPID() << ", " << Commandlist.at(commandcounter)->getAction() << ": Variable " << Commandlist.at(commandcounter)->getVarID() << ", Value " << Commandlist.at(commandcounter)->getToValue() << endl;
                            m10.unlock();
                        }
                        else {
                            m10.lock();
                            cout << "Time " << clk->getTime() << ", P" << tempProc->getPID() << ", " << Commandlist.at(commandcounter)->getAction() << ": Variable " << Commandlist.at(commandcounter)->getVarID() << endl;
                            m10.unlock();
                        }
                        APICall = Commandlist.at(commandcounter)->getActionID();
                        //cout << APICall << endl;
                        if (APICall == 1) {
                            valuetostore = Commandlist.at(commandcounter)->getToValue();
                            varID = Commandlist.at(commandcounter)->getVarID();
                        }
                        else {
                            varID = Commandlist.at(commandcounter)->getVarID();
                        }
                        commandcounter++;

                        switch (APICall)
                        {
                            case 1:
                                store(MemoryStorage,VirtualMemory, varID, valuetostore);
                                //cout << MemoryStorage.at(2)->getValue() << endl;
                                break;
                            case 2:
                                release(MemoryStorage, varID);
                                //cout << MemoryStorage.at(varPos)->getValue() << endl;
                                break;
                        }
                        //std::this_thread::sleep_for(std::chrono::milliseconds(a));
                        b -= a;
                        while (clk->getTime() != (d + a)) {} //////
                        m6.lock();
                        //cout << "P" << tempProc->getPID() << " Finished command at " << clk->getTime() << endl;
                        m6.unlock();
                    }
                    else {
                        //cout << "P" << tempProc->getPID() << " Burst time left is " <<b<< endl;
                        m7.lock();
                        //cout << "P" << tempProc->getPID() << " Not enough time to execute next command, sleeping for the rest of burst time. " << endl;
                        m7.unlock();
                        //std::this_thread::sleep_for(std::chrono::milliseconds(b));
                        b -= b;
                        //cout << b << endl;
                    }

                }
                else {
                    if (c > 0) {
                        //std::this_thread::sleep_for(std::chrono::milliseconds(a));
                        b -= a;
                        while (clk->getTime() != (d + a)) {} //////
                        m6.lock();
                        //cout << "P" << tempProc->getPID() << " Finished command at " << clk->getTime() << endl;
                        m6.unlock();
                    }
                    else {
                        //cout << "P" << tempProc->getPID() << " Burst time left is " <<b<< endl;
                        m7.lock();
                        //cout << "P" << tempProc->getPID() << " Not enough time to execute next command, sleeping for the rest of burst time. " << endl;
                        m7.unlock();
                        //std::this_thread::sleep_for(std::chrono::milliseconds(b));
                        b -= b;
                        //cout << b << endl;
                    }
                }
            }
        }
    }
    m10.lock();
    cout << "Time " << clk->getTime() << ", P" << tempProc->getPID() << ", Terminated"/*" Ended by CPU" << this_thread::get_id() << endl;
    m10.unlock();
    //cout << "TESTING HERE" << endl;
    done = true;
    //cout << "Bool done is now " << done << endl;
    //cout << "Program will crash." << endl;

}*/

/*void Process::start(Process * tempProc, Clock * clk, int &timer, bool& done, bool& checkifFirsttimer, vector<Memory*>& MemoryStorage, vector<MMU*>& Commandlist, vector<Memory*>& VirtualMemory) {
    //cout << tempProc->getPID() << "Entered start()" << endl;
    t = new std::thread(&Process::run, this, std::ref(tempProc), std::ref(clk), std::ref(timer), std::ref(done), std::ref(checkifFirsttimer), std::ref(MemoryStorage), std::ref(Commandlist), std::ref(VirtualMemory));
}*/

void Process::run(Process *tempProc, Clock *clk, int &timer, bool& done,bool& checkifFirsttimer) {
    int b = tempProc->getbT();
    m5.lock();
    cout << "Time " << clk->getTime() << ", P" << tempProc->getPID() << " Started" << endl;
    m5.unlock();
    timer += tempProc->getbT();
    srand(time(NULL));

    if (checkifFirsttimer) {
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
                //cout << "P" << tempProc->getPID() << "'s C: " << c << ", B: " << b << ", A: " << a << endl;
                //if (b > 0) {

                        //std::this_thread::sleep_for(std::chrono::milliseconds(a));
                        b -= a;
                        while (clk->getTime() != (d + a)) {} //////
                        m6.lock();
                        //cout << "P" << tempProc->getPID() << " Finished command at " << clk->getTime() << endl;
                        m6.unlock();
                        //cout << "P" << tempProc->getPID() << " Burst time left is " <<b<< endl;
                        m7.lock();
                        //cout << "P" << tempProc->getPID() << " Not enough time to execute next command, sleeping for the rest of burst time. " << endl;
                        m7.unlock();
                        //std::this_thread::sleep_for(std::chrono::milliseconds(b));
                        b -= b;
                        //cout << b << endl;
                }
            }
        done = true;
        checkifFirsttimer = false;
    }
    else if (!checkifFirsttimer) {
        int a = 0;
        int c = 0;
        int sumOa = 0;
        int d = 0;
        int APICall = 0;
        int valuetostore = 0;
        int varID = 0;
        while (clk->getTime() != timer) {
            while(b>0){
                a = rand() % 200;
                sumOa += a;
                d = clk->getTime();
                c = b - a;
                        //cout << "P" << tempProc->getPID() << " Burst time left is " <<b<< endl;
                        m7.lock();
                        //cout << "P" << tempProc->getPID() << " Not enough time to execute next command, sleeping for the rest of burst time. " << endl;
                        m7.unlock();
                        //std::this_thread::sleep_for(std::chrono::milliseconds(b));
                        b -= b;
                        //cout << b << endl;
                    }
                }
            }
    m10.lock();
    cout << "Time " << clk->getTime() << ", P" << tempProc->getPID() << ", Terminated"/*" Ended by CPU" << this_thread::get_id()*/ << endl;
    m10.unlock();
    //cout << "TESTING HERE" << endl;
    done = true;
    //cout << "Bool done is now " << done << endl;
    //cout << "Program will crash." << endl;

}

void Process::start(Process * tempProc, Clock * clk, int &timer, bool& done, bool& checkifFirsttimer) {
    //cout << tempProc->getPID() << "Entered start()" << endl;
    t = new std::thread(&Process::run, this, std::ref(tempProc), std::ref(clk), std::ref(timer), std::ref(done), std::ref(checkifFirsttimer));
}

void Process::setaT(int aT){
    Arrival_Time = aT;
}

void Process::setbT(int bT){
    Burst_Time = bT;
}

void Process::setPID(int pid){
    PID = pid;
}

int Process::getaT(){
    return Arrival_Time;
}

int Process::getbT(){
    return Burst_Time;
}

//void Process::store(vector<Memory*>& MemoryStorage,vector<Memory*>& VirtualMemory, /*int & varPos,*/ int & varID, int& valuetostore) {
    /*bool found = false;
    int counter = 0;
    int free = 0;
    bool gotfree = false;
    bool storedinVM = false;

    //Check if there's free room in memory
    for (int j = 0; j < MemoryStorage.size(); j++) {
        if (MemoryStorage.at(j)->getVarID() == 0) {
            counter++;
        }
    }

    //Fetch first free position to store
    //for (int k = 0; k < MemoryStorage.size(); k++) {
    //	if(!gotfree){
    //		if (MemoryStorage.at(k)->getVarID() == 0) {
    //			free = k;
    //			gotfree = true;
    //		}
    //	}
    //}

    //If Main memory is full, check if variable is stored in main memory. otherwise do something else.
    //cout << counter << endl;
    if (counter ==0) {
        for (int i = 0; i < MemoryStorage.size(); i++) {
            if (varID == MemoryStorage.at(i)->getVarID()) {
                MemoryStorage.at(i)->setVarID(varID);
                MemoryStorage.at(i)->setValue(valuetostore);
                found = true;
                break;
            }
        }
        if (!found) {


            // Add case that main memory is full and variable does not exist in main memory.
            // Store into virtual
            if (VirtualMemory.size() == 0) {
                Memory tempMemory;
                tempMemory.setAgeint(0);
                tempMemory.setValue(valuetostore);
                tempMemory.setVarID(varID);
                VirtualMemory.push_back(&tempMemory);
                cout << "Inside Virtual Memory : " << VirtualMemory.at(0)->getVarID() << " " << VirtualMemory.at(0)->getValue() << endl;
            }
            //else {
            //	for (int i = 0; i < VirtualMemory.size(); i++) {
            //		if (varID == VirtualMemory.at(i)->getVarID()) {
            //			VirtualMemory.at(i)->setVarID(varID);
            //			VirtualMemory.at(i)->setValue(valuetostore);
            //			found = true;
            //			break;
            //		}
            //	}
            //	if (!found) {
            //		for (int i = 0; i < VirtualMemory.size(); i++) {
            //			if (VirtualMemory.at(i)->getVarID() == 0) {
            //				VirtualMemory.at(i)->setVarID(varID);
            //				VirtualMemory.at(i)->setValue(valuetostore);
            //
            //				storedinVM = true;
            //				break;
            //			}
            //		}
            //		if (!storedinVM) {
            //			Memory tempMemory;
            //			tempMemory.setAgeint(0);
            //			tempMemory.setValue(valuetostore);
            //			tempMemory.setVarID(varID);
            //			VirtualMemory.push_back(&tempMemory);
            //		}
            //	}
            //}

            //for (int i = 0; i < VirtualMemory.size(); i++) {
            //	cout << "Inside Virtual Memory : " << VirtualMemory.at(i)->getVarID() << " " << VirtualMemory.at(i)->getValue() << endl;
            //}
            //std::thread t1(&print, std::ref(VirtualMemory));
            cout << "PLEASE STORE IN VM" << endl;

            //t1.join();



        }
    }
    else if (counter > 0) {
        bool stored = false;
        for (int i = 0; i < MemoryStorage.size(); i++) {
            if (MemoryStorage.at(i)->getVarID()== varID) {
                MemoryStorage.at(i)->setVarID(varID);
                MemoryStorage.at(i)->setValue(valuetostore);
                stored = true;
                break;
            }
            //Main memory : [3 , ] [0 , ] [3 , ]
        }
        if (!stored) {
            for (int i = 0; i < MemoryStorage.size(); i++) {
                if (MemoryStorage.at(i)->getVarID() == 0) {
                    MemoryStorage.at(i)->setVarID(varID);
                    MemoryStorage.at(i)->setValue(valuetostore);
                    break;
                }
            }
        }

    }

    //for (int i = 0; i < MemoryStorage.size(); i++) {
    //cout << "Variable " << MemoryStorage.at(i)->getVarID() << endl;
    //}

    //MemoryStorage.at(varPos)->setValue(valuetostore);
}*/

/*void Process::release(vector<Memory*>& MemoryStorage, int varID) {
    for (int i = 0; i < MemoryStorage.size(); i++) {
        if (MemoryStorage.at(i)->getVarID() == varID) {
            MemoryStorage.at(i)->setVarID(0);
        }
    }
}

void print(vector<Memory*>&VirtualMemory) {
    for (int i = 0; i < VirtualMemory.size(); i++) {
        cout << "Inside Virtual Memory : " << VirtualMemory.at(i)->getVarID() << " " << VirtualMemory.at(i)->getValue() << endl;
    }
}*/

int Process::getPID(){
    return PID;
}

Process::~Process() {
    //t->join();
    //dtor
}
