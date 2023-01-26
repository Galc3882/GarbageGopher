//
PyObject *initServoKit();

//
void sendServoCommand(PyObject *kit, int servoIndex, int angle);

//
void openLid(PyObject *kit);

//
void closeLid(PyObject *kit);