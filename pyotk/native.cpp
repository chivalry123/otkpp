
#include "constraints.hpp"
#include "functions.hpp"
#include "io_utils.hpp"
#include "numpy_utils.hpp"
#include "solver_utils.hpp"
#include "solvers.hpp"
#include "std_utils.hpp"
#include "stop_crit.hpp"
#include "test_functions.hpp"
#include "time_utils.hpp"

#include <boost/numeric/ublas/io.hpp>
#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#ifdef WITH_GSL
#include <gsl/gsl_multimin.h>
#endif
#include <iostream>
#include <string>

using namespace boost::python;

static const unsigned int MAX_NUM_ITER = 50000;
static const unsigned long long MIN_TOTAL_TIME = 1e9;

struct SolverInfo
{
  int m;    // the number of points the solver produces each iteration step
};

struct SolverInput
{
  boost::shared_ptr< Constraints > C;               // the constraints for the problem
  Function objFunc;                                 // the objective function
  bool hasConstraints;                              // does the test problem have constraints
  SolverInfo solverInfo;                            // information about the used solver
  boost::shared_ptr< StoppingCriterion > stopCrit;  // the stopping criterion
};

struct SolverResults_Python
{
  SolverInput input;      // the used input parameters
  bool converged;         // was the chosen stopping criterion satisfied
  double f_final;         // the final estimate for the minimum function value
  int n;                  // number of variables
  int num_iter;           // number of used iterations
  int num_func_eval;      // number of function evaluations
  int num_grad_eval;      // number of gradient evaluations
  list states;            // list of solver states for each iteration step
  double time;            // used time
  double term_val;        // the final termination test value
  tuple x_final;          // the final estimate for the minimizer
};

// DEPRECATED
SolverResults_Python minimize(NativeSolver &solver,
                              const Solver::Setup &solverSetup,
                              Function &objFunc,
                              const StoppingCriterion &stopCrit,
                              const vector< double > &x0,
                              const Constraints &C,
                              int verbosity,
                              bool timeTest)
{
  throw std::runtime_error("obsolete function called");
  
  bool converged;
  unsigned int k;
  unsigned int numRuns = 0;
  SolverResults_Python results;
  unsigned long long startTime = 0;
  NativeSolver::IterationStatus status;
  unsigned long long totalTime = 0;
  const Constraints *C_;
  /*with_custodian_and_ward_postcall<0, 1, reference_existing_object >::
    apply< const NativeSolver::State & >::type solver_state_converter;*/
  //return_by_value::apply< const NativeSolver::State & >::type solver_state_converter;
  
  array::set_module_and_type("numpy", "ndarray");
  
  if(verbosity >= 2)
    printResultsTableHeader(solver, objFunc);
  
  if(timeTest == false)
    objFunc.enableEvalCounting();
  else
    objFunc.disableEvalCounting();
  
  C_ = getOTKConstraintsInstance(C);
  
  do
  {
    k = 0;
    converged = false;
    status = NativeSolver::ITERATION_CONTINUE;
    
    solver.setup(objFunc, x0, solverSetup, *C_);
    
    if(timeTest == true)
      startTime = getTime();
    else
      objFunc.resetEvalCounters();
    
    if(!solver.isExternalSolver())
    {
      do
      {
        if(verbosity >= 2)
          printResultsTableRow(k, solver, objFunc);
        
        if(timeTest == false)
          results.states.append(boost::shared_ptr< NativeSolver::State >(solver.getState().clone()));
          //results.states.append(handle<>(solver_state_converter(solver.getState())));
        
        if(status == NativeSolver::ITERATION_CONTINUE && converged)
          break;
        
        status = solver.iterate();
        
        if(status != NativeSolver::ITERATION_CONTINUE && 
           stopCrit.test(solver) == false)
        {
          converged = false;
          break;
        }
        k++;
        
        converged = stopCrit.test(solver);
      }
      while(status == NativeSolver::ITERATION_CONTINUE && 
            converged == false && k < MAX_NUM_ITER);
    }
    else
      throw std::runtime_error("external solvers are not yet supported by the driver routine");
    
    if(timeTest == false)
      results.states.append(boost::shared_ptr< NativeSolver::State >(solver.getState().clone())); /*results.states.append(handle<>(solver_state_converter(solver.getState())));*/
    else
    {
      totalTime += getTime() - startTime;
      numRuns++;
    }
  }
  while(timeTest == true && totalTime < MIN_TOTAL_TIME);
  
  if(verbosity >= 1)
    printSummary(k, solver, objFunc);
  
  results.input.objFunc = objFunc;
  if(solver.supportsConstraints(*C_))
  {
    results.input.hasConstraints = true;
    results.input.C = 
      boost::shared_ptr< Constraints >(
      dynamic_cast< Constraints * >(C.clone()));
    /*if(typeid(C.get()) == typeid(BoundConstraints_Python *))
      results.input.C = dynamic_cast< const BoundConstraints_Python & >(*C.get());*/
  }
  else
    results.input.hasConstraints = false;
  results.input.solverInfo.m = solver.getM();
  
  results.converged     = converged;
  results.f_final       = solver.getFVal();
  results.n             = objFunc.getN();
  results.num_iter      = std::min(k + 1, MAX_NUM_ITER);
  results.num_func_eval = solver.getNumFuncEval();
  results.num_grad_eval = solver.getNumGradEval();
  results.term_val      = stopCrit.getTestValue(solver);
  results.x_final       = numpy_utils::vector_to_tuple::convert_boost(solver.getX());
  
  if(timeTest == true)
  {
    if(converged)
      results.time = ((double)totalTime) / numRuns / 1000.0;
    else
      results.time = 0;
  }
  
  delete C_;
  
  return results;
}

// TODO: This converter is a hack.
// It should be able to convert to (gsl_multimin_fxxminimizer *).
// But it doesn't work because of a limitation in Boost.Python.
/*struct gsl_name_to_solver
{
  gsl_name_to_solver()
  {
    boost::python::converter::registry::push_back(
      &convertible, &construct,
      boost::python::type_id< const gsl_multimin_fdfminimizer_type * >());
  }
  
  static void *convertible(PyObject *obj_ptr)
  {
    if(!PyString_Check(obj_ptr))
      return NULL;
    return obj_ptr;
  }
  
  static void construct(
    PyObject *obj_ptr,
    boost::python::converter::rvalue_from_python_stage1_data *data)
  {
    std::string str = PyString_AsString(obj_ptr);
    
    void *storage = (
      (boost::python::converter::
       rvalue_from_python_storage< const gsl_multimin_fdfminimizer_type * > *)
      data)->storage.bytes;
    new (storage)(const gsl_multimin_fdfminimizer_type **);
    const gsl_multimin_fdfminimizer_type **t = 
      (const gsl_multimin_fdfminimizer_type **)storage;
    if(str == "gsl_conjugate_fr")
      *t = gsl_multimin_fdfminimizer_conjugate_fr;
    else if(str == "gsl_conjugate_pr")
      *t = gsl_multimin_fdfminimizer_conjugate_pr;
    /*else if(std == gsl_nmsimplex)
      *t = *gsl_multimin_fminimizer_nmsimplex;*/ // TODO
/*else if(str == "gsl_vector_bfgs")
      *t = gsl_multimin_fdfminimizer_vector_bfgs;
    else if(str == "gsl_vector_bfgs2")
      *t = gsl_multimin_fdfminimizer_vector_bfgs2;
    else if(str == "gsl_steepest_descent")
      *t = gsl_multimin_fdfminimizer_steepest_descent;
    
    data->convertible = storage;
  }
};*/

static void init_converters()
{
  to_python_converter< vector< double >, numpy_utils::vector_to_tuple >();
  to_python_converter< matrix< double >, numpy_utils::matrix_to_ndarray >();
  numpy_utils::tuple_to_vector();
  numpy_utils::ndarray_to_matrix();
  std_utils::tuple_to_vector< double >();
  std_utils::tuple_to_vector< std::string >();
  std_utils::tuple_to_vector< BoundConstraints::BoundType >();
  to_python_converter< std::vector< double >, std_utils::vector_to_tuple< double > >();
  to_python_converter< std::vector< std::string >, std_utils::vector_to_tuple< std::string > >();
  to_python_converter< std::vector< boost::shared_ptr< NativeSolver::State > >,
    std_utils::vector_to_tuple< boost::shared_ptr< NativeSolver::State > > >();
  to_python_converter< std::vector< boost::shared_ptr< Solver::Results > >,
    std_utils::vector_to_tuple< boost::shared_ptr< Solver::Results > > >();
  to_python_converter< NativeSolver, solver_state_to_python >();
}

BOOST_PYTHON_MODULE(native)
{
  init_constraints();
  init_functions();
  init_solvers();
  init_stop_crit();
  init_test_functions();
  
  class_< SolverInfo >("solver_info")
    .def_readonly("m", &SolverInfo::m);
  class_< SolverInput >("SolverInput")
    .def_readonly("C",               &SolverInput::C)
    .def_readonly("has_constraints", &SolverInput::hasConstraints)
    .def_readonly("objfunc",         &SolverInput::objFunc)
    .def_readonly("solver_info",     &SolverInput::solverInfo)
    .def_readonly("stop_crit",       &SolverInput::stopCrit);
  class_< SolverResults_Python >("SolverResults")
    .def_readonly("input",         &SolverResults_Python::input)
    .def_readonly("f_final",       &SolverResults_Python::f_final)
    .def_readonly("converged",     &SolverResults_Python::converged)
    .def_readonly("n",             &SolverResults_Python::n)
    .def_readonly("num_iter",      &SolverResults_Python::num_iter)
    .def_readonly("num_func_eval", &SolverResults_Python::num_func_eval)
    .def_readonly("num_grad_eval", &SolverResults_Python::num_grad_eval)
    .def_readonly("states",        &SolverResults_Python::states)
    .def_readonly("time",          &SolverResults_Python::time)
    .def_readonly("term_val",      &SolverResults_Python::term_val)
    .def_readonly("x_final",       &SolverResults_Python::x_final);
  def("minimize", minimize,
    (boost::python::arg("C") = NoConstraints(),
    boost::python::arg("verbosity") = 0,
    boost::python::arg("timeTest") = false));
  
  init_converters();
  
  import_array();
  array::set_module_and_type("numpy", "ndarray");
}
