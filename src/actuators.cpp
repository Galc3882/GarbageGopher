#include "pyheader.cpp"

PyObject *initServoKit()
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

void sendServoCommand(PyObject *kit, int servoIndex, int angle)
{
    // kit.servo[servoIndex].angle = angle
    PyObject *servo = PyObject_GetAttrString(kit, "servo");
    PyObject *servoIndexPy = PyLong_FromLong(servoIndex);
    PyObject *servoAtIndex = PyObject_GetItem(servo, servoIndexPy);
    PyObject *anglePy = PyLong_FromLong(angle);
    PyObject_SetAttrString(servoAtIndex, "angle", anglePy);
}

void openLid(PyObject *kit)
{
    // kit.servo[0].angle = 150
    sendServoCommand(kit, 0, 150);
}

void closeLid(PyObject *kit)
{
    // kit.servo[0].angle = 50
    sendServoCommand(kit, 0, 50);
}
