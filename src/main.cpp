#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <JetsonGPIO.h>
#include <Python.h>
#include <sensors.hpp>
#include <actuators.hpp>

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

    // create ultrasonic sensor object
    UltSensor::UltrasonicSensor ultrasonicSensor = UltSensor::UltrasonicSensor();

    // open new thread for reading
    std::thread ultrasonicThread (&UltSensor::UltrasonicSensor::ultrasonicDistanceThread, &ultrasonicSensor);

    for (int i = 0; i < 60; i++)
    {
        // print distance
        // std::cout << ultrasonicSensor.getDistance() << std::endl;
        ultrasonicSensor.printDebug();
        // sleep for 1/4 second
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // stop reading
    ultrasonicSensor.stopReading();
    // join thread
    ultrasonicThread.join();

    return 0;
}