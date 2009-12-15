
#ifndef NUMPY_UTILS_H

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/python.hpp>
#include <numpy/ndarrayobject.h>

using namespace boost::numeric::ublas;
using namespace boost::python;
using namespace boost::python::numeric;

namespace numpy_utils
{
  static array make_array(int m, int n, double fill)
  {
    list l;
    for(int i = 0; i < m * n; i++)
      l.append(fill);
    tuple t = tuple(l);
    array a(t);
    a.resize(make_tuple(m, n));
    
    return a;
  }
  
  static array zeros(int m, int n)
  {
    return make_array(m, n, 0.0);
  }
  
  struct ndarray_to_matrix
  {
    ndarray_to_matrix()
    {
      boost::python::converter::registry::push_back(
        &convertible, &construct,
        boost::python::type_id< matrix< double > >());
    }
    
    static void *convertible(PyObject *obj_ptr)
    {
      if(!PyArray_Check(obj_ptr))
        return NULL;
      // TODO: check that the number of dimensions is two
      return obj_ptr;
    }
    
    static void construct(
      PyObject *obj_ptr,
      boost::python::converter::rvalue_from_python_stage1_data *data)
    {
      const int m = PyArray_DIM(obj_ptr, 0);
      const int n = PyArray_DIM(obj_ptr, 1);
      
      void *storage = (
        (boost::python::converter::rvalue_from_python_storage< matrix< double > > *)
        data)->storage.bytes;
      new (storage) matrix< double >(m, n);
      matrix< double > *M = (matrix< double > *)storage;
      double *array_data = (double *)PyArray_DATA(obj_ptr);
      
      for(int i = 0; i < m; i++)
        for(int j = 0; j < n; j++)
          (*M)(i, j) = array_data[j + i*n];
      data->convertible = storage;
    }
  };
  
  struct matrix_to_ndarray
  {
    static PyObject* convert(const matrix< double > &M)
    {
      array a = zeros(M.size1(), M.size2());
      
      for(int i = 0; i < M.size1(); i++)
        for(int j = 0; j < M.size2(); j++)
          a[make_tuple(i, j)] = M(i, j);
      
      return incref(a.ptr());
    }
  };
  
  struct tuple_to_vector
  {
    tuple_to_vector()
    {
      boost::python::converter::registry::push_back(
        &convertible, &construct,
        boost::python::type_id< vector< double > >());
    }
    
    static void *convertible(PyObject *obj_ptr)
    {
      if(!PyTuple_Check(obj_ptr))
        return NULL;
      return obj_ptr;
    }
    
    static void construct(
      PyObject *obj_ptr,
      boost::python::converter::rvalue_from_python_stage1_data *data)
    {
      const int n = PyTuple_Size(obj_ptr);
      
      void *storage = (
        (boost::python::converter::rvalue_from_python_storage< vector< double > > *)
        data)->storage.bytes;
      new (storage) vector< double >(n);
      vector< double > *v = (vector< double > *)storage;
      
      for(int i = 0; i < n; i++)
        (*v)(i) = extract< double >(PyTuple_GetItem(obj_ptr, i));
      data->convertible = storage;
    }
  };

  struct vector_to_tuple
  {
    static tuple convert_boost(const vector< double > &v)
    {
      list l;
      for(int i = 0; i < v.size(); i++)
        l.append(v[i]);
      return tuple(l);
    }
    
    static PyObject* convert(const vector< double > &v)
    {
      list l;
      for(int i = 0; i < v.size(); i++)
        l.append(v[i]);
      
      return incref(tuple(l).ptr());
    }
  };
}

#define NUMPY_UTILS_H

#endif
