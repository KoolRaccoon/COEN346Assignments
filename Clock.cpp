#include "Clock.hpp"
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

//Initialize a new clock object that starts at time 0
Clock::Clock() {
    Counter=0;
}

//Increments the time on the clock after sleeping for 1ms
void Clock::IncrememtCounter () {
    while(Counter <= 10000){
        mutex.lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        mutex.unlock();
        Counter+=1;
    }
}

//Starts a new thread of a clockIncrementation
void Clock::StartClock() {
    t = new std::thread(&Clock::IncrememtCounter, this);
}

//Returns the current time
int Clock::getTime (){
    return Counter;
}

Clock::~Clock(){}
