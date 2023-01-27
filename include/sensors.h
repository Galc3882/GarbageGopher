// declaration of the class UltrasonicSensor
class UltrasonicSensor
{
public:
    // constructor
    UltrasonicSensor();
    // destructor
    ~UltrasonicSensor();
    // check if processing reading
    bool processingReading;
    // trigger pin
    int triggerPin;
    // echo pin
    int echoPin;
    // get distance in cm
    int getDistance();
    // trigger reading function
    void triggerReading();
    // callback function for echo pin
    void echoRisingCallback();
    // callback function for echo pin on falling edge
    void echoFallingCallback();
private:
    // last echo rise time
    std::chrono::high_resolution_clock::time_point lastEchoRiseTime;
    // last reading time
    std::chrono::high_resolution_clock::time_point lastReadingTime;
    // last readings
    int lastReadings[5];
    // processing reading
};