#pragma once
#ifndef SENSORS_HPP
#define SENSORS_HPP

// declare namespace for ultrasonic sensor
namespace UltSensor
{
    // declaration of the class UltrasonicSensor
    class UltrasonicSensor
    {
        // Uses pins 19 for trigger and 21 for echo
        // Send pulse and wait for echo using interupts, then calculate distance
        // Ranging Distance : 2cm – 400 cm
        // Resolution : 0.3 cm
        // Measuring Angle: 30 degree

    public:
        // constructor
        UltrasonicSensor();
        // destructor
        ~UltrasonicSensor();
        // debug function
        void printDebug();
        // get distance in cm
        int getDistance();
        // thread for reading
        void ultrasonicDistanceThread();
        // stop reading
        void stopReading();

    private:
        // set trigger pin
        int triggerPin;
        // set echo pin
        int echoPin;
        // check if processing reading
        bool processingReading;
        // check if reading
        bool reading;
        // last sensor readings
        int lastReadings[5];
        // last reading time
        std::chrono::high_resolution_clock::time_point lastReadingTime;

        // trigger reading
        void triggerReading();
        // wait for echo pin to go high and then low to calculate distance
        void waitForReading();
    };
} // namespace UltSensor

#endif // SENSORS_HPP