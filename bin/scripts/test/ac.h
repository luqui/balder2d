#include "../../../include/input.h"


static PyMethodDef BalderAIMethods[] = {
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

class ArtificialController{
public:
    static void Initialize(){
        Py_InitializeEx(0);
        // set up the python environment to find the scrpits
        PyRun_SimpleString("import sys\n");
        PyRun_SimpleString("sys.path.append('scripts')\n");
        PyObject *balder_module = Py_InitModule("balder", BalderAIMethods);
        PyModule_AddIntConstant(balder_module, "INPUT_LEFT", Balder::LEFT);
        PyModule_AddIntConstant(balder_module, "INPUT_RIGHT", Balder::RIGHT);
        PyModule_AddIntConstant(balder_module, "INPUT_STICK", Balder::STICK);
        PyModule_AddIntConstant(balder_module, "INPUT_FIRE", Balder::FIRE);
    }
    static void ShutDown(){
        Py_Finalize();
    }

    #ifdef Py_PYTHON_H  // so gamemanager doesn't see this python related stuff
    static PyObject* py_IsWall(PyObject *self, PyObject *args);
    static PyObject* py_IsLineOfSight(PyObject *self, PyObject *args);
    #endif
};
