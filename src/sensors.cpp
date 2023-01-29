#include <thread>
#include <iostream> // remove
#include <JetsonGPIO.h>
#include "sensors.hpp"

// constructor
UltSensor::UltrasonicSensor::UltrasonicSensor()
{
    // setup GPIO
    GPIO::setmode(GPIO::BOARD);
    // set pins to trigger and echo
    this->triggerPin = 19;
    this->echoPin = 21;
    // set trigger pin to output
    GPIO::setup(this->triggerPin, GPIO::OUT);
    // set echo pin to input
    GPIO::setup(this->echoPin, GPIO::IN);
    // set trigger pin to low
    GPIO::output(this->triggerPin, GPIO::LOW);
    //set processing reading to false
    this->processingReading = false;
    // set reading to false
    this->reading = false;
    // set last readings time to now
    this->lastReadingTime = std::chrono::high_resolution_clock::now();
    // set last readings to 0
    for (int i = 0; i < 5; i++)
    {
        this->lastReadings[i] = 0;
    }
}

// destructor
UltSensor::UltrasonicSensor::~UltrasonicSensor()
{
    // stop reading
    this->stopReading();
    // cleanup GPIO
    GPIO::cleanup();
}

// debug function
void UltSensor::UltrasonicSensor::printDebug()
{
    // print time
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->lastReadingTime).count() << " ms" << std::endl;
    // get distance
    int distance = this->getDistance();
    // print distance
    std::cout << "Distance: " << distance << " cm" << std::endl;
    // print last readings
    std::cout << "Last Readings: ";
    for (int i = 0; i < 5; i++)
    {
        std::cout << this->lastReadings[i] << " ";
    }
    std::cout << std::endl;
}

// get distance in cm
int UltSensor::UltrasonicSensor::getDistance()
{
    // get average of last 5 readings
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        // if no null value, add to sum
        if (this->lastReadings[i] != 0)
        {
            sum += this->lastReadings[i];
        }
    }
    return sum / 5;
}

// thread for reading
void UltSensor::UltrasonicSensor::ultrasonicDistanceThread()
{
    this->reading = true;
    while (reading)
    {
        // trigger reading
        this->triggerReading();
        // sleep for 50 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void UltSensor::UltrasonicSensor::stopReading()
{
    this->reading = false;
}

// trigger reading function
void UltSensor::UltrasonicSensor::triggerReading()
{
    // check if processing reading or last reading was less than 80 ms ago
    if (this->processingReading || std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->lastReadingTime).count() < 80)
    {
        return;
    }

    // processing reading
    this->processingReading = true;
    // set trigger pin to high
    GPIO::output(this->triggerPin, GPIO::HIGH);
    // wait 10 microseconds (https://web.eece.maine.edu/~zhu/book/lab/HC-SR04%20User%20Manual.pdf)
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    // set trigger pin to low
    GPIO::output(this->triggerPin, GPIO::LOW);

    waitForReading();
}

// wait for echo pin to go high and then low to calculate distance
void UltSensor::UltrasonicSensor::waitForReading()
{
    // wait for echo pin to go high with timeout (40 ms no obsticle)
    auto start = std::chrono::high_resolution_clock::now();
    while (GPIO::input(21) == GPIO::LOW)
    {
        // check if timeout
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
        if (duration.count() > 40000)
        {
            this->processingReading = false;
            return;
        }
    }

    // measure time between high and low
    start = std::chrono::high_resolution_clock::now();
    while (GPIO::input(21) == GPIO::HIGH)
    {
        // check if timeout
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
        if (duration.count() > 40000)
        {
            this->processingReading = false;
            return;
        }
    }

    // calculate distance
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
    int distance = duration.count() / 58;

    // add to last readings
    for (int i = 4; i > 0; i--)
    {
        this->lastReadings[i] = this->lastReadings[i - 1];
    }
    this->lastReadings[0] = distance;
    // not processing reading
    this->processingReading = false;
    // set reading time
    this->lastReadingTime = now;
}
