// Initialize the servo kit
PyObject *initServoKit();

// Send a command to the servo 
void sendServoCommand(PyObject *kit, int servoIndex, int angle);

// Open the lid of the garbage bin
void openLid(PyObject *kit);

// Close the lid of the garbage bin
void closeLid(PyObject *kit);