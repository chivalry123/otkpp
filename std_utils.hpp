
#ifndef STDUTILS_HPP

#include <vector>

namespace std_utils
{
  struct tuple_to_vector
  {
    tuple_to_vector()
    {
      boost::python::converter::registry::push_back(
        &convertible, &construct,
        boost::python::type_id< std::vector< double > >());
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
        (boost::python::converter::rvalue_from_python_storage< std::vector< double > > *)
        data)->storage.bytes;
      new (storage) std::vector< double >(n);
      std::vector< double > *v = (std::vector< double > *)storage;
      
      for(int i = 0; i < n; i++)
        (*v)[i] = extract< double >(PyTuple_GetItem(obj_ptr, i));
      data->convertible = storage;
    }
  };
  
  struct vector_to_tuple
  {
    static PyObject* convert(const std::vector< double > &v)
    {
      list l;
      for(int i = 0; i < v.size(); i++)
        l.append(v[i]);
      
      return incref(tuple(l).ptr());
    }
  };
}

#define STDUTILS_HPP

#endif
