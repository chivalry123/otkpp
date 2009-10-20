
#ifndef SOLVER_UTILS_H

#include <Python.h>

struct solver_state_to_python
{
  static PyObject* convert(const NativeSolver &solver)
  {
    PyObject *classDict = PyDict_New();
    PyObject *className = PyString_FromString("SolverState");
    
    PyObject *x_tuple = 
      numpy_utils::vector_to_tuple::convert(solver.getX());
    //Py_INCREF(x_tuple);
    PyDict_SetItem(classDict, PyString_FromString("x"), x_tuple);
    
    PyObject *X_matrix = 
      numpy_utils::matrix_to_ndarray::convert(solver.getXArray());
    PyDict_SetItem(classDict, PyString_FromString("X"), X_matrix);
    
    PyDict_SetItem(classDict, PyString_FromString("f"), PyFloat_FromDouble(solver.getFVal()));
    
    if(solver.usesGradient())
    {
      PyObject *g_tuple = 
        numpy_utils::vector_to_tuple::convert(solver.getGradient());
      //Py_INCREF(g_tuple);
      PyDict_SetItem(classDict, PyString_FromString("g"), g_tuple);
    }
    
    PyObject *state_class = PyClass_New(NULL, classDict, className);
    
    Py_INCREF(state_class);
    return state_class;
  }
};

#define SOLVER_UTILS_H

#endif
