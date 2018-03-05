#include <iostream>
#include "Process.h"
#include <vector>
#include <thread>
#include "Scheduler.h"
#include "Clock.h"
#include <mutex>

//mutex mu;

Scheduler * scheduler;

using namespace std;

//void takeProcess(vector<Process*> &ProcessQ, Clock& clk); //Trying to pass in Clock object. Doesn't work...
/* Maybe you shouldn't pass Clock clk, but just "clk" instead since you're not defining the function, only declaring it*/
//Process* tempProc;
//Process* veryfirstProc;


// Right now, I initiated the clock object. However, when I try to pass it to the function, it crashes. Idk what the issue is.
// std::ref( ) wraps the object we're passing so that it is passed by reference, because threads by nature takes a copy of the object instead of actual, thus it won't reflect actual object. Need tlo pass by
int main()
{
    
    scheduler->main();
    
    /*
     
     
    Clock clk;
    clk.StartClock();
    cout<< "Time: " << clk.getTime() <<endl;
    Process *pt1;
    Process *pt2;
    Process p1(1,1000,2500);
    Process p2(2,2500,1000);
    Process p3(3,3500,1500);

    Process* ProcessList[2];
    vector<Process*> ProcessQ;
    //for (int i = 1; i<sizeof(ProcessList); i++)
    ProcessQ.push_back(&p1);
    ProcessQ.push_back(&p2);
    ProcessQ.push_back(&p3);

    std::thread CPU1(&takeProcess, std::ref(ProcessQ), std::ref(clk)); // Attempting to passe Clock object to thread1
    std::thread CPU2(&takeProcess, std::ref(ProcessQ), std::ref(clk)); // Attempting to passe Clock object to thread2
    
    CPU1.join();
    CPU2.join();

    ProcessList[0] = p1;
    ProcessList[1] = p2;
    pt1 = & ProcessList[0];
    pt2 = & ProcessList[1];

    cout << "Process pointer 1 is pointing at index 0 and its process' arrival time is " << pt1->getaT() << endl;
    cout << "Process pointer 2 is pointing at index 1, and its process' arrival time is " << pt2->getaT() <<endl;
*/    return 0;
}

/*
void takeProcess(vector<Process*> &ProcessQ, Clock& clk){
    veryfirstProc = ProcessQ.front();
    int counter = 1;
    int time = 0;
    bool startProcess = false;
    
    while(!startProcess){
        if(clk.getTime()==veryfirstProc->getaT()){
            startProcess=true;
        }
    }

    if(startProcess){
            time = veryfirstProc->getaT();
    }

    while(ProcessQ.size()>0){
    
        std::lock_guard<std::mutex> lock(mu);
        tempProc = ProcessQ.front();
        cout << "Time : " << clk.getTime() << ", P" << tempProc->getPID() << " Started" << endl;
        
        time += tempProc->getbT();
    
        while(clk.getTime()!= time){}

        cout << "Time : " << clk.getTime() << ", P"<< tempProc->getPID() <<" ended. " << "current thread: " << std::this_thread::get_id() << endl;
    
        for(int i = 0; i<ProcessQ.size(); i++){ //For loop to pop the front process out of the vector and shifting the objects.
            if(i<ProcessQ.size()-1){
                ProcessQ.at(i) = ProcessQ.at(i+1);
            }else{
                ProcessQ.at(i)= NULL;
            }
    }

    ProcessQ.pop_back();
    counter++;
    mu.unlock();
    }

}
*/