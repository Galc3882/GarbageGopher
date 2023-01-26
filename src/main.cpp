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
    GPIO::setmode(GPIO::BOARD);

    // create ultrasonic sensor object
    UltrasonicSensor ultrasonicSensor;

    // set current time
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

    // set variables for reading time
    std::chrono::high_resolution_clock::time_point readingTime = std::chrono::high_resolution_clock::now();
    // read once
    ultrasonicSensor.triggerReading();

    // benchmarking counter
    int counter = 0;

    // loop for 1 minute
    while (currentTime - std::chrono::high_resolution_clock::now() < std::chrono::minutes(1))
    {
        // measure distance every 20th of a second
        if (!ultrasonicSensor.processingReading && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - readingTime).count() >= 20){
            ultrasonicSensor.triggerReading();
            readingTime = std::chrono::high_resolution_clock::now();
            // print distance
            std::cout << ultrasonicSensor.getDistance() << std::endl;
        }

        // increment counter
        counter++;
    }

    // print benchmarking results
    std::cout << "Benchmarking results: " << counter << std::endl;

    // cleanup GPIO
    GPIO::cleanup();

    return 0;
}