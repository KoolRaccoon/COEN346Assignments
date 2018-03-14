#include <iostream>
#include "Process.hpp"
#include <vector>
#include <thread>
#include "Scheduler.hpp"
#include "Clock.hpp"
#include "Memory.hpp"
#include "MMU.hpp"


using namespace std;

// Right now, I initiated the clock object. However, when I try to pass it to the function, it crashes. Idk what the issue is.
// std::ref( ) wraps the object we're passing so that it is passed by reference, because threads by nature takes a copy of the object instead of actual, thus it won't reflect actual object. Need tlo pass by
int main()
{
    Scheduler scheduler;

    scheduler.main();
    return 0;
}
