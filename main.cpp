#include <iostream>
#include "Process.hpp"
#include <vector>
#include <thread>
#include "Scheduler.hpp"
#include "Clock.hpp"


using namespace std;

int main() {
    Scheduler scheduler;

    scheduler.main();
    return 0;
}
