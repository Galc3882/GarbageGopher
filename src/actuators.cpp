#include "actuators.hpp"

ServoActuator::ServoActuator()
{
    : kp_(0), ki_(0), kd_(0), previous_error_(0), integral_(0) {}
}

PyObject *ServoActuator::initServoKit()
{
    // from adafruit_servokit import ServoKit
    const char *adafruit_servokit = "adafruit_servokit";
    const char *ServoKit = "ServoKit";
    PyObject *servokit = loadComponentFromModule(const_cast<char *>(adafruit_servokit), const_cast<char *>(ServoKit));

    // kit = ServoKit(channels=16)
    PyObject *value = Py_BuildValue("{s:i}", "channels", 16);
    PyObject *emptyTuple = Py_BuildValue("()");
    return PyObject_Call(servokit, emptyTuple, value);
}

void ServoActuator::sendServoCommand(PyObject *kit, int servoIndex, int angle)
{
    // kit.servo[servoIndex].angle = angle
    PyObject *servo = PyObject_GetAttrString(kit, "servo");
    PyObject *servoIndexPy = PyLong_FromLong(servoIndex);
    PyObject *servoAtIndex = PyObject_GetItem(servo, servoIndexPy);
    PyObject *anglePy = PyLong_FromLong(angle);
    PyObject_SetAttrString(servoAtIndex, "angle", anglePy);
}

void ServoActuator::openLid(PyObject *kit)
{
    // kit.servo[0].angle = 150
    sendServoCommand(kit, 0, 150);
}

void ServoActuator::closeLid(PyObject *kit)
{
    // kit.servo[0].angle = 50
    sendServoCommand(kit, 0, 50);
}

void ServoActuator::moveForward(PyObject *kit, int speed)
{
    int leftSpeed = 90 + speed;
    int rightSpeed = 90 + speed;
    sendServoCommand(kit, 1, leftSpeed);  // Assuming motor 1 is left
    sendServoCommand(kit, 2, rightSpeed); // Assuming motor 2 is right
}

void ServoActuator::moveBackward(PyObject *kit, int speed)
{
    int leftSpeed = 90 - speed;
    int rightSpeed = 90 - speed;
    sendServoCommand(kit, 1, leftSpeed);
    sendServoCommand(kit, 2, rightSpeed);
}

void ServoActuator::turnLeft(PyObject *kit, int speed)
{
    int leftSpeed = 90 - speed;
    int rightSpeed = 90 + speed;
    sendServoCommand(kit, 1, leftSpeed);
    sendServoCommand(kit, 2, rightSpeed);
}

void ServoActuator::turnRight(PyObject *kit, int speed)
{
    int leftSpeed = 90 + speed;
    int rightSpeed = 90 - speed;
    sendServoCommand(kit, 1, leftSpeed);
    sendServoCommand(kit, 2, rightSpeed);
}

void ServoActuator::stop(PyObject *kit)
{
    sendServoCommand(kit, 1, 90); // Stops left motor
    sendServoCommand(kit, 2, 90); // Stops right motor
}

void ServoActuator::setPIDParameters(double kp, double ki, double kd)
{
    kp_ = kp;
    ki_ = ki;
    kd_ = kd;
}

double ServoActuator::getPIDControl(double setpoint, double current_value)
{
    double error = setpoint - current_value;
    integral_ += error;
    double derivative = error - previous_error_;
    previous_error_ = error;

    return kp_ * error + ki_ * integral_ + kd_ * derivative;
}