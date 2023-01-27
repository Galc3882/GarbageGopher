#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <thread>
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

    // create ultrasonic sensor object
    UltrasonicSensor ultrasonicSensor;

    // open new thread for reading
    std::thread ultrasonicThread(&UltrasonicSensor::ultrasonicDistanceThread, &ultrasonicSensor);

    // wait for 1 minute and count every 1/4 second
    for (int i = 0; i < 240; i++)
    {
        // print distance
        std::cout << ultrasonicSensor.getDistance() << std::endl;
        // sleep for 1/4 second
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }


    return 0;
}