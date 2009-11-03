
#include "constraints.hpp"
#include "io_utils.hpp"
#include "numpy_utils.hpp"
#include "solver_utils.hpp"
#include "time_utils.hpp"

#include <otkpp/function/Function.h>
#include <otkpp/localsolvers/ExternalSolver.h>
#include <otkpp/localsolvers/native/ConjGradMT.h>
#include <otkpp/localsolvers/native/HookeJeeves.h>
#include <otkpp/localsolvers/native/LinminBFGS.h>
#include <otkpp/localsolvers/native/SteihaugSR1.h>
#include <otkpp/localsolvers/native/DoglegBFGS.h>
#include <otkpp/localsolvers/native/NativeSolver.h>
#include <otkpp/localsolvers/native/PARTAN.h>
#include <otkpp/localsolvers/native/linmin/Fletcher.h>
#include <otkpp/localsolvers/native/linmin/MoreThuente.h>
#include <otkpp/stopcrit/CompoundStoppingCriterion.h>
#include <otkpp/stopcrit/FDistToMinTest.h>
#include <otkpp/stopcrit/GradNormTest.h>
#include <otkpp/stopcrit/MaxNumIterTest.h>
#include <otkpp/stopcrit/StoppingCriterion.h>
#include <otkpp/stopcrit/XDistToMinTest.h>
#include <otkpp/testproblems/MGHTestFunction.h>
#ifdef WITH_GSL
#include <otkpp/localsolvers/gslsolvers/GSLFSolver.h>
#include <otkpp/localsolvers/gslsolvers/GSLFDFSolver.h>
#endif
#ifdef WITH_FORTRAN
#include <otkpp/localsolvers/lbfgsb/LBFGSB.h>
#endif

#include <boost/numeric/ublas/io.hpp>
#include <boost/python.hpp>
#ifdef WITH_GSL
#include <gsl/gsl_multimin.h>
#endif
#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>

using namespace boost::python;

static const unsigned int MAX_NUM_ITER = 50000;
static const unsigned long long MIN_TOTAL_TIME = 1e9;

struct SolverInfo
{
  int m;    // the number of points the solver produces each iteration step
};

struct SolverInput
{
  boost::shared_ptr< Constraints > C;  // the constraints for the problem
  Function objFunc;                    // the objective function
  bool hasConstraints;                 // does the test problem have constraints
  SolverInfo solverInfo;               // information about the used solver
  // TODO: stopping criterion?
};

struct SolverResults_Python
{
  SolverInput input;        // the used input parameters
  bool converged;           // was the chosen stopping criterion satisfied
  double f_final;           // the final estimate for the minimum function value
  int n;                    // number of variables
  int num_iter;             // number of used iterations
  int num_func_eval;        // number of function evaluations
  int num_grad_eval;        // number of gradient evaluations
  list states;              // list of solver states for each iteration step
  double time;              // used time
  double term_val;          // the final termination test value
  tuple x_final;            // the final estimate for the minimizer
};

SolverResults_Python minimize(NativeSolver &solver,
                              const SolverSetup &solverSetup,
                              Function &objFunc,
                              const StoppingCriterion &stopCrit,
                              const vector< double > &x0,
                              const Constraints &C,
                              int verbosity,
                              bool timeTest)
{
  bool converged;
  unsigned int k;
  unsigned int numRuns = 0;
  SolverResults_Python results;
  unsigned long long startTime = 0;
  NativeSolver::IterationStatus status;
  unsigned long long totalTime = 0;
  const Constraints *C_;
  
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
          results.states.append(solver);
        
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
      results.states.append(solver);
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

void init_functions()
{
#ifdef WITH_LIBMATHEVAL
  class_< Function >("Function",
    init< const std::string &, optional< Function::DerivEvalType > >())
    .def("get_n", &Function::getN);
#else
  class_< Function >("Function")
    .def("get_n", &Function::getN);
#endif
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

void init_solvers()
{
  class_< SolverSetup, boost::noncopyable >("SolverSetup", no_init);
  class_< NativeSolver, boost::noncopyable >("Solver", no_init)
    .def("get_m", &NativeSolver::getM)
    .def("get_name", &NativeSolver::getName)
    .def("get_n", &NativeSolver::getN);
  enum_< ConjGradMT::Type >("ConjGradType")
    .value("FR", ConjGradMT::FLETCHER_REEVES)
    .value("PR", ConjGradMT::POLAK_RIBIERE);
  class_< ConjGradMT, bases< NativeSolver > >("ConjGradMT",
    init< ConjGradMT::Type >());
#ifdef WITH_GSL
  class_< GSLFSolver, bases< NativeSolver > >("GSLfsolver",
    init< const std::string & >());
  class_< GSLFDFSolver, bases< NativeSolver > >("GSLfdfsolver",
    init< const std::string &, optional< Function::DerivEvalType > >());
#endif
  class_< HookeJeeves, bases< NativeSolver > >("HookeJeeves");
  class_< LinminSetup, boost::noncopyable >("LinminSetup", no_init);
  class_< FletcherSetup, bases< LinminSetup > >(
    "FletcherSetup", init< optional< double, double, double, double > >());
  class_< MoreThuenteSetup, bases< LinminSetup > >(
    "MoreThuenteSetup", init< optional< double, double, double, double > >());
  enum_< LinminBFGS::LinMinType >("BFGSLmType")
    .value("fletcher", LinminBFGS::FLETCHER)
    .value("morethuente", LinminBFGS::MORE_THUENTE);
  class_< LinminBFGSSetup, bases< SolverSetup > >("LinminBFGSSetup",
    init< optional< const LinminSetup &, const matrix< double > & > >());
  class_< LinminBFGS, bases< NativeSolver > >("LinminBFGS",
    init< optional< LinminBFGS::LinMinType, int > >());
  class_< DoglegBFGS, bases< NativeSolver > >("DoglegBFGS");
  class_< PARTAN, bases< NativeSolver > >("PARTAN");
  class_< SteihaugSR1, bases< NativeSolver > >("SteihaugSR1");
  class_< SolverSetup, boost::noncopyable >("SolverSetup", no_init);
  class_< DefaultSolverSetup, bases< SolverSetup > >("DefaultSolverSetup");
#ifdef WITH_GSL
  class_< GSLFSolver_setup, bases< SolverSetup > >("GSLFSolverSetup",
    init< const vector< double > & >());
  class_< GSLFDFSolver_setup, bases< SolverSetup > >("GSLFDFSolverSetup",
    init< double, double >());
#endif
#ifdef WITH_FORTRAN
  class_< LBFGSB, bases< NativeSolver > >("LBFGSB");
#endif
  //register_ptr_to_python< boost::shared_ptr< Solver > >();
}

void init_stop_crit()
{
  class_< StoppingCriterion, boost::noncopyable >("StoppingCriterion", no_init)
    .def(self + self);
  class_< CompoundStoppingCriterion, bases< StoppingCriterion > >("CompoundStoppingCriterion")
    .def(self + self);
  class_< FDistToMinTest, bases< StoppingCriterion > >("FDistToMinTest", 
    init< double, double, bool >((arg("f_min"), arg("eps"), arg("relative") = false)));
  class_< GradNormTest, bases< StoppingCriterion > >("GradNormTest", init< double >(arg("eps")));
  class_< MaxNumIterTest, bases< StoppingCriterion > >("MaxNumIterTest", init< int >(arg("n")));
  class_< XDistToMinTest, bases< StoppingCriterion > >("XDistToMinTest", 
    init< const vector< double > &, double, bool >((arg("x_min"), arg("eps"), arg("relative") = false)));
}

void init_test_functions()
{
  class_< PowellBadlyScaled, bases< Function > >(
    "PowellBadlyScaled", init< Function::DerivEvalType >());
  class_< BrownBadlyScaled, bases< Function > >(
    "BrownBadlyScaled", init< Function::DerivEvalType >());
  class_< Beale, bases< Function > >(
    "Beale", init< Function::DerivEvalType >());
  class_< HelicalValley, bases< Function > >(
    "HelicalValley", init< Function::DerivEvalType >());
  class_< Gaussian, bases< Function > >(
    "Gaussian", init< Function::DerivEvalType >());
  class_< ExtendedRosenbrock, bases< Function > >(
    "ExtendedRosenbrock", init< int, Function::DerivEvalType >());
  class_< Gulf, bases< Function > >(
    "Gulf", init< int, Function::DerivEvalType >());
  class_< Box, bases< Function > >(
    "Box", init< int, Function::DerivEvalType >());
  class_< Wood, bases< Function > >(
    "Wood", init< Function::DerivEvalType >());
  class_< BrownDennis, bases< Function > >(
    "BrownDennis", init< int, Function::DerivEvalType >());
  class_< BiggsEXP6, bases< Function > >(
    "BiggsEXP6", init< int, Function::DerivEvalType >());
  class_< Watson, bases< Function > >(
    "Watson", init< int, Function::DerivEvalType >());
  class_< ExtendedPowellSingular, bases< Function > >(
    "ExtendedPowellSingular", init< int, Function::DerivEvalType >());
  class_< PenaltyFunctionI, bases< Function > >(
    "PenaltyFunctionI", init< int, Function::DerivEvalType >());
  class_< PenaltyFunctionII, bases< Function > >(
    "PenaltyFunctionII", init< int, Function::DerivEvalType >());
  class_< VariablyDimensioned, bases< Function > >(
    "VariablyDimensioned", init< int, Function::DerivEvalType >());
  class_< Trigonometric, bases< Function > >(
    "Trigonometric", init< int, Function::DerivEvalType >());
  class_< ChebyQuad, bases< Function > >(
    "ChebyQuad", init< int, int, Function::DerivEvalType >());
}

BOOST_PYTHON_MODULE(native)
{
  init_constraints();
  init_functions();
  init_solvers();
  init_stop_crit();
  init_test_functions();
  numpy_utils::tuple_to_vector();
  to_python_converter< vector< double >, numpy_utils::vector_to_tuple >();
  to_python_converter< matrix< double >, numpy_utils::matrix_to_ndarray >();
  //std_utils::tuple_to_vector();
  //to_python_converter< std::vector< double >, std_utils::vector_to_tuple >();
  to_python_converter< NativeSolver, solver_state_to_python >();
  class_< SolverInfo >("solver_info")
    .def_readonly("m", &SolverInfo::m);
  class_< SolverInput >("SolverInput")
    .def_readonly("C",               &SolverInput::C)
    .def_readonly("has_constraints", &SolverInput::hasConstraints)
    .def_readonly("objfunc",         &SolverInput::objFunc)
    .def_readonly("solver_info",     &SolverInput::solverInfo);
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
}
