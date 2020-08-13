#include "Tasks/Task.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char** argv){
    auto lam = [](int a, int b){std::cout << "a=" << a << " b=" << b;};
    Task task(false, false, lam, 3, 5);
    task.start();
    sleep(1);
    //sleep(1);
}