#include <thread>
#include <iostream>
#include <cmath>
#include <JetsonGPIO.h>
#include "sensors.hpp"

// Initializes GPIOs and sets initial conditions
UltSensor::UltrasonicSensor::UltrasonicSensor()
{
    // GPIO initialization
    GPIO::setmode(GPIO::BOARD);
    this->triggerPin = 19;
    this->echoPin = 21;
    GPIO::setup(this->triggerPin, GPIO::OUT);
    GPIO::setup(this->echoPin, GPIO::IN);
    GPIO::output(this->triggerPin, GPIO::LOW);

    // Initialize other member variables
    this->processingReading = false;
    this->reading = false;
    this->lastReadingTime = std::chrono::high_resolution_clock::now();
}

// Destructor to handle cleanup
UltSensor::UltrasonicSensor::~UltrasonicSensor()
{
    this->stopReading();
    GPIO::cleanup();
}

// Get distance based on last readings, also filters out the outliers
int UltSensor::UltrasonicSensor::getDistance()
{
    int sum = 0;
    int arrSize = this->lastReadings.size();

    for (int val : lastReadings)
        sum += val;

    int mean = sum / arrSize;

    // Calculate standard deviation
    int stddev = 0;
    for (int val : lastReadings)
        stddev += (val - mean) * (val - mean);
    stddev = sqrt(stddev / (arrSize - 1));

    // Identify and remove outliers
    int index = 0;
    int maxDev = 0;
    for (int i = 0; i < arrSize; i++)
    {
        int dev = abs(this->lastReadings[i] - mean);
        if (dev > maxDev)
        {
            maxDev = dev;
            index = i;
        }
    }

    if (maxDev > stddev)
    {
        sum -= this->lastReadings[index];
        arrSize--;
    }

    return sum / arrSize;
}

// Thread function to initiate sensor readings
void UltSensor::UltrasonicSensor::ultrasonicDistanceThread()
{
    this->reading = true;
    while (reading)
    {
        this->triggerReading();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// Stop the reading process
void UltSensor::UltrasonicSensor::stopReading()
{
    this->reading = false;
}

// Initiate a reading cycle
void UltSensor::UltrasonicSensor::triggerReading()
{
    if (this->processingReading || std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->lastReadingTime).count() < 80)
        return;

    this->processingReading = true;
    GPIO::output(this->triggerPin, GPIO::HIGH);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    GPIO::output(this->triggerPin, GPIO::LOW);

    waitForReading();
}

// Await the reading result and compute distance
void UltSensor::UltrasonicSensor::waitForReading()
{
    auto start = std::chrono::high_resolution ::clock::now();

    // Wait for the echo pin to go HIGH
    while (GPIO::input(this->echoPin) == GPIO::LOW)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);

        // If we've waited too long (40ms) without getting a HIGH, exit function
        if (duration.count() > 40000)
        {
            this->processingReading = false;
            return;
        }
    }

    // Time the duration for which the echo pin remains HIGH
    start = std::chrono::high_resolution_clock::now();
    while (GPIO::input(this->echoPin) == GPIO::HIGH)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);

        // If the duration is too long (40ms), exit function
        if (duration.count() > 40000)
        {
            this->processingReading = false;
            return;
        }
    }

    // Compute distance using the time echo pin was HIGH
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
    int distance = duration.count() / 58;

    // Update the last readings array
    for (int i = this->lastReadings.size() - 1; i > 0; i--)
    {
        this->lastReadings[i] = this->lastReadings[i - 1];
    }
    this->lastReadings[0] = distance;

    // Update other member variables accordingly
    this->processingReading = false;
    this->lastReadingTime = now;
}
