#pragma once

#include <thread>
#include <vector>
#include <chrono>
#include <JetsonGPIO.h>

namespace UltSensor
{
    class UltrasonicSensor
    {
    public:
        // Constructor
        UltrasonicSensor();

        // Destructor
        ~UltrasonicSensor();

        // Stop reading from sensor
        void stopReading();

        // Start reading from sensor using a separate thread
        void ultrasonicDistanceThread();

        // Trigger a reading from the sensor
        void triggerReading();

        // Get the current distance measured by the sensor
        int getDistance();

    private:
        // GPIO pin for triggering the sensor reading
        int triggerPin;
        
        // GPIO pin for reading the echo signal
        int echoPin;

        // Flag indicating if a reading is currently being processed
        bool processingReading;

        // Flag indicating if the reading thread is active
        bool reading;

        // Time when the last reading was taken
        std::chrono::time_point<std::chrono::high_resolution_clock> lastReadingTime;

        // Circular buffer to store the last 5 readings
        std::vector<int> lastReadings = std::vector<int>(5);

        // Private method to wait for a reading to complete and compute the distance
        void waitForReading();
    };
}
