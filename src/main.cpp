#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <JetsonGPIO.h>
#include <Python.h>
#include <sensors.h>
#include <actuators.h>

void testLid()
{
    // Set the PYTHONPATH environment variable to the current directory
    setenv("PYTHONPATH", ".", 1);
    // Initialize the Python Interpreter
    Py_Initialize();

    // Initialize the ServoKit
    PyObject *kit = initServoKit();

    // Open the lid
    openLid(kit);

    // Wait 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Close the lid
    closeLid(kit);

    // Wait 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Finalize the Python Interpreter
    Py_Finalize();
}

int main(void)
{
    // testLid();

    // setup GPIO
    setupGPIOcpp();

    // measure distance every 1 second for 20 seconds
    for (int i = 0; i < 200; i++)
    {
        std::cout << ultrasonicDistance() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // cleanup GPIO
    GPIO::cleanup();

    return 0;
}