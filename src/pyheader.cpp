#include <Python.h>

// Load and return a Python module given its name.
PyObject *loadModule(char *name)
{
    // Convert C string to Python string object.
    PyObject *pName = PyUnicode_FromString(name);

    // Import the Python module using its name.
    PyObject *pModule = PyImport_Import(pName);

    // Clean up the Python string object to release memory.
    Py_DECREF(pName);

    return pModule;
}

// Load and return a specific component (e.g., function or class) from a given Python module.
PyObject *loadComponentFromModule(char *module, char *component)
{
    // Load the desired Python module.
    PyObject *pModule = loadModule(module);

    // Retrieve the dictionary object (namespace) of the module.
    PyObject *pDict = PyModule_GetDict(pModule);

    // Get the desired component from the module's dictionary.
    PyObject *pComponent = PyDict_GetItemString(pDict, component);

    // Clean up the loaded module and dictionary objects.
    Py_DECREF(pModule);
    Py_DECREF(pDict);

    return pComponent;
}
