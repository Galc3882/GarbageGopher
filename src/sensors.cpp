#include <JetsonGPIO.h>

int ultrasonicDistance()
{
    // Uses pins 19 for trigger and 21 for echo
    // Send pulse and wait for echo, then calculate distance
    // Ranging Distance : 2cm – 400 cm
    // Resolution : 0.3 cm
    // Measuring Angle: 30 degree

    // set trigger pin to high
    GPIO::output(19, GPIO::HIGH);
    // wait 10 microseconds (https://web.eece.maine.edu/~zhu/book/lab/HC-SR04%20User%20Manual.pdf)
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    // set trigger pin to low
    GPIO::output(19, GPIO::LOW);

    // wait for echo pin to go high with timeout (40 ms no obsticle)
    auto start = std::chrono::high_resolution_clock::now();
    while (GPIO::input(21) == GPIO::LOW)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
        if (duration.count() > 40000)
        {
            return -1;
        }
    }

    // measure time between high and low
    start = std::chrono::high_resolution_clock::now();
    while (GPIO::input(21) == GPIO::HIGH)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
        if (duration.count() > 40000)
        {
            return -1;
        }
    }

    // calculate distance
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start);
    return duration.count() / 58;
}

void setupGPIOcpp()
{
    // set pin numbering mode to BOARD
    GPIO::setmode(GPIO::BOARD);
    // set trigger pin to output
    GPIO::setup(19, GPIO::OUT);
    // set echo pin to input
    GPIO::setup(21, GPIO::IN);
}