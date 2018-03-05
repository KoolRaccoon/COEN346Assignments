#include "Clock.h"
#include <string>
#include <thread>
#include <chrono>
#include <iostream>


Clock::Clock()
{
    Counter=0;
}

void Clock::IncrememtCounter () {
    while(Counter <= 10000){
    mutex.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
//std::cout << "Time elapsed: " << Counter << std::endl;
    mutex.unlock();
    Counter+=100;
    }
}

void Clock::StartClock() {
    t = new std::thread(&Clock::IncrememtCounter, this);
}

int Clock::getTime (){
    return Counter;
}

Clock::~Clock()
{
    t->join();
}
