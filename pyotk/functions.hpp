
#ifndef FUNCTIONS_HPP

#include <otkpp/function/Function.h>
#include <boost/python.hpp>

void init_functions()
{
#ifdef WITH_LIBMATHEVAL
  class_< Function >("Function", 
    init< const std::string &, optional< Function::DerivEvalType > >())
    .def("get_n", &Function::getN)
    .def("get_symbolic_expression", &Function::getSymbolicExpression)
    .def("get_variable_names", &Function::getVariableNames, return_value_policy< return_by_value >())
#else
  class_< Function >("Function")
    .def("get_n", &Function::getN)
#endif
    .def("has_symbolic_expression", &Function::hasSymbolicExpression);
  enum_< Function::FuncEvalType >("FuncEvalType")
#ifdef WITH_LIBMATHEVAL
    .value("symbolic", Function::SYMBOLIC)
#endif
    .value("compiled", Function::COMPILED);
  enum_< Function::DerivEvalType >("DerivEvalType")
#ifdef WITH_LIBMATHEVAL
    .value("symbolic", Function::DERIV_SYMBOLIC)
#endif
    .value("fdiff_central_2", Function::DERIV_FDIFF_CENTRAL_2)
    .value("fdiff_central_4", Function::DERIV_FDIFF_CENTRAL_4)
    .value("fdiff_forward", Function::DERIV_FDIFF_FORWARD);
}

#define FUNCTIONS_HPP

#endif
