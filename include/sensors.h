// declaration of the class UltrasonicSensor
class UltrasonicSensor
{
public:
    // constructor
    UltrasonicSensor();
    // destructor
    ~UltrasonicSensor();
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
    std::chrono::time_point<std::chrono::system_clock> lastReadingTime;
    
    // trigger reading
    void triggerReading();
    // wait for echo pin to go high and then low to calculate distance
    void waitForReading();
};