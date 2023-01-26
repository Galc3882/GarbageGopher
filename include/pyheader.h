// Loads a python module and returns a pointer to it
PyObject *loadModule(char *name);

// Loads a python component from a module and returns a pointer to it
PyObject *loadComponentFromModule(char *module, char *component);