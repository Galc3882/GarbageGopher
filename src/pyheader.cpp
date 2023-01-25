#include <Python.h>

PyObject *loadModule(char *name)
{
    // import name
    PyObject *pName = PyUnicode_FromString(name);
    PyObject *pModule = PyImport_Import(pName);
    Py_DECREF(pName);
    return pModule;
}

PyObject *loadComponentFromModule(char *module, char *component)
{
    // from module import component
    PyObject *pModule = loadModule(module);
    PyObject *pDict = PyModule_GetDict(pModule);
    PyObject *pComponent = PyDict_GetItemString(pDict, component);
    Py_DECREF(pModule);
    Py_DECREF(pDict);
    return pComponent;
}

