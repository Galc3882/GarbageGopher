#ifndef ACTUATORS_HPP
#define ACTUATORS_HPP

#include <Python.h>
#include <pyheader.hpp>

class ServoActuator {
public:
    ServoActuator();

    // Servo kit
    PyObject* initServoKit();
    void sendServoCommand(PyObject* kit, int servoIndex, int angle);
    void openLid(PyObject* kit);
    void closeLid(PyObject* kit);

    // Differential Drive
    void moveForward(PyObject* kit, int speed);
    void moveBackward(PyObject* kit, int speed);
    void turnLeft(PyObject* kit, int speed);
    void turnRight(PyObject* kit, int speed);
    void stop(PyObject* kit);

    // Additional functionalities for PID control
    void setPIDParameters(double kp, double ki, double kd);
    double getPIDControl(double setpoint, double current_value);

private:
    // PID controller variables
    double kp_, ki_, kd_;
    double previous_error_;
    double integral_;
};

#endif // ACTUATORS_HPP
