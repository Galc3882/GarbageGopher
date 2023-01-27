#include <thread>
#include <JetsonGPIO.h>

// declare class for ultrasonic sensor object
class UltrasonicSensor
{
public:
    // Uses pins 19 for trigger and 21 for echo
    // Send pulse and wait for echo using interupts, then calculate distance
    // Ranging Distance : 2cm – 400 cm
    // Resolution : 0.3 cm
    // Measuring Angle: 30 degree

    // constructor
    UltrasonicSensor()
    {
        // set pins to trigger and echo
        this->triggerPin = 19;
        this->echoPin = 21;
        // set trigger pin to output
        GPIO::setup(this->triggerPin, GPIO::OUT);
        // set echo pin to input
        GPIO::setup(this->echoPin, GPIO::IN);
        // set trigger pin to low
        GPIO::output(this->triggerPin, GPIO::LOW);
    }

    // destructor
    ~UltrasonicSensor();

    // check if processing reading
    bool processingReading = false;

    // get distance in cm
    int getDistance()
    {
        // get average of last 5 readings
        int sum = 0;
        for (int i = 0; i < 5; i++)
        {
            sum += this->lastReadings[i];
        }
        return sum / 5;
    }

    // trigger reading function
    void triggerReading()
    {
        // check if processing reading or last reading was less than 80 ms ago
        if (this->processingReading || std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - this->lastReadingTime).count() < 80)
        {
            return;
        }

        // set trigger pin to high
        GPIO::output(this->triggerPin, GPIO::HIGH);
        // wait 10 microseconds (https://web.eece.maine.edu/~zhu/book/lab/HC-SR04%20User%20Manual.pdf)
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        // set trigger pin to low
        GPIO::output(this->triggerPin, GPIO::LOW);
        // processing reading
        this->processingReading = true;
    }

        // callback function for echo pin
    void echoRisingCallback()
    {
        // measure time between high and low
        this->lastEchoRiseTime = std::chrono::high_resolution_clock::now();
    }

    // callback function for echo pin on falling edge
    void echoFallingCallback()
    {
        // measure time between high and low
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - this->lastEchoRiseTime);
        // calculate distance in cm
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

private:
    // set trigger pin
    int triggerPin;
    // set echo pin
    int echoPin;
    // last sensor readings
    int lastReadings[5];
    // last reading time
    std::chrono::high_resolution_clock::time_point lastReadingTime;
    // last trigger time
    std::chrono::high_resolution_clock::time_point lastEchoRiseTime;
};
