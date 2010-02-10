
#ifndef SOLVERS_HPP

#include <otkpp/localsolvers/Solver.h>
#include <otkpp/localsolvers/native/ConjGradMT.h>
//#include <otkpp/localsolvers/native/DFQAS.h>
#include <otkpp/localsolvers/native/DSQA.h>
#include <otkpp/localsolvers/native/HookeJeeves.h>
#include <otkpp/localsolvers/native/LinminBFGS.h>
#include <otkpp/localsolvers/native/LRWWSimplex.h>
#include <otkpp/localsolvers/native/SteihaugSR1.h>
#include <otkpp/localsolvers/native/DoglegBFGS.h>
#include <otkpp/localsolvers/native/NativeSolver.h>
#include <otkpp/localsolvers/native/PARTAN.h>
#include <otkpp/localsolvers/native/linmin/Fletcher.h>
#include <otkpp/localsolvers/native/linmin/MoreThuente.h>
#ifdef WITH_GSL
#include <otkpp/localsolvers/gslsolvers/GSLFSolver.h>
#include <otkpp/localsolvers/gslsolvers/GSLFDFSolver.h>
#endif
#ifdef WITH_FORTRAN
#include <otkpp/localsolvers/lbfgsb/LBFGSB.h>
#include <otkpp/localsolvers/lmbm/LMBM.h>
#endif

#include <boost/python.hpp>

void init_solvers()
{
  class_< QuadInterp >("QuadInterp")
    .add_property("g", make_function(&QuadInterp::getG, return_value_policy< return_by_value >()))
    .add_property("H", make_function(&QuadInterp::getH, return_value_policy< return_by_value >()))
    .add_property("X", make_function(&QuadInterp::getX, return_value_policy< return_by_value >()));
  
  {
    scope solve_scope(class_< Solver, boost::noncopyable >("Solver", no_init)
      .def("get_m", &Solver::getM)
      .def("get_name", &Solver::getName)
      .def("get_n", &Solver::getN)
      .def("solve", make_function(&Solver::solve, return_value_policy< return_by_value >())));
    class_< Solver::Setup, boost::noncopyable >("Setup", no_init)
      .def_readonly("C", &Solver::Setup::C)
      .def_readonly("f", &Solver::Setup::f)
      .def_readonly("m", &Solver::Setup::m)
      .def_readonly("n", &Solver::Setup::n);
    class_< Solver::DefaultSetup, bases< Solver::Setup > >("DefaultSetup");
    class_< Solver::Results >("Results")
      .def_readonly("converged", &Solver::Results::converged)
      .def_readonly("f_min", &Solver::Results::fMin)
      .def_readonly("num_func_eval", &Solver::Results::numFuncEval)
      .def_readonly("num_grad_eval", &Solver::Results::numGradEval)
      .def_readonly("num_iter", &Solver::Results::numIter)
      .def_readonly("setup", &Solver::Results::setup)
      .def_readonly("term_val", &Solver::Results::termVal)
      .def_readonly("time", &Solver::Results::time)
      .add_property("x_min", make_getter(&Solver::Results::xMin, return_value_policy< return_by_value >()));
  }
  
  {
    scope nativesolver_scope(class_< NativeSolver, bases< Solver >, boost::noncopyable >("NativeSolver", no_init));
    class_< NativeSolver::State, boost::noncopyable >("State", no_init)
      .def_readonly("fx", &NativeSolver::State::fx)
      .add_property("x", make_getter(&NativeSolver::State::x, return_value_policy< return_by_value >()))
      .add_property("X", make_getter(&NativeSolver::State::X, return_value_policy< return_by_value >()));
    class_< NativeSolver::Results, bases< Solver::Results > >("Results")
      .add_property("states", make_getter(&NativeSolver::Results::states, return_value_policy< return_by_value >()));
  }
  
  class_< GradientSolverBase, bases< NativeSolver >, boost::noncopyable >("GradientSolverBase", no_init);
  
  {
    scope nativegradientsolver_scope(class_< NativeGradientSolver, bases< GradientSolverBase >, boost::noncopyable >("NativeGradientSolver", no_init));
    class_< NativeGradientSolver::State, bases< NativeSolver::State >, boost::noncopyable >("State", no_init)
      .def_readonly("alpha", &NativeGradientSolver::State::alpha)
      .add_property("gx", make_getter(&NativeGradientSolver::State::g, return_value_policy< return_by_value >()));
  }
  
  enum_< ConjGradMT::Type >("ConjGradType")
    .value("FR", ConjGradMT::FLETCHER_REEVES)
    .value("PR", ConjGradMT::POLAK_RIBIERE);
  class_< ConjGradMT, bases< NativeGradientSolver > >("ConjGradMT",
    init< ConjGradMT::Type >());
  
  /*{
    scope dfqas_scope(class_< DFQAS, bases< NativeSolver > >("DFQAS"));
    class_< DFQAS::State, bases< NativeSolver::State > >("State");
  }*/
  
  {
    scope dsqa_scope(class_< DSQA, bases< NativeSolver > >("DSQA"));
    class_< DSQA::State, bases< NativeSolver::State > >("State")
      .def_readonly("delta", &DSQA::State::delta)
      .def_readonly("model", &DSQA::State::model);
  }
  
  class_< HookeJeeves, bases< NativeSolver > >("HookeJeeves");
  class_< DoglegBFGS, bases< NativeSolver > >("DoglegBFGS");
  
  {
    scope linminbfgs_scope(class_< LinminBFGS, bases< NativeGradientSolver > >("LinminBFGS",
      init< optional< LinminBFGS::LinMinType, int > >()));
    enum_< LinminBFGS::LinMinType >("LinminType")
      .value("fletcher", LinminBFGS::FLETCHER)
      .value("morethuente", LinminBFGS::MORE_THUENTE);
    class_< LinminBFGS::Setup, bases< Solver::Setup > >("Setup",
      init< optional< const LineMinimizer::Setup &, const matrix< double > & > >());
    class_< LinminBFGS::State, bases< NativeGradientSolver::State > >("State")
      .add_property("H", make_getter(&LinminBFGS::State::H, return_value_policy< return_by_value >()));
  }
  
  class_< LRWWSimplex, bases< NativeSolver > >("LRWWSimplex");
  class_< PARTAN, bases< NativeGradientSolver > >("PARTAN");
  class_< SteihaugSR1, bases< NativeGradientSolver > >("SteihaugSR1");

  {
    scope lineminimizer_scope(class_< LineMinimizer, boost::noncopyable >("LineMinimizer", no_init));
    class_< LineMinimizer::Setup, boost::noncopyable >("Setup", no_init);
    class_< LineMinimizer::DefaultSetup, bases< LineMinimizer::Setup > >("DefaultSetup");
  }
  
  {
    scope fletcher_scope(class_< Fletcher >("Fletcher"));
    class_< Fletcher::Setup, bases< LineMinimizer::Setup > >(
      "Setup", init< optional< double, double, double, double > >());
  }
  
  {
    scope morethuente_scope(class_< MoreThuente >("MoreThuente"));
    class_< MoreThuente::Setup, bases< LineMinimizer::Setup > >(
      "Setup", init< optional< double, double, double, double > >());
  }

#ifdef WITH_GSL
  {
    scope gslfsolver_scope(class_< GSLFSolver, bases< NativeSolver > >("GSLfsolver",
      init< const std::string & >()));
    class_< GSLFSolver::Setup, bases< Solver::Setup > >("Setup",
      init< const vector< double > & >());
    class_< GSLFSolver::State, bases< NativeSolver::State > >("State");
  }
  
  {
    scope gslfdfsolver_scope(class_< GSLFDFSolver, bases< NativeGradientSolver > >("GSLfdfsolver",
      init< const std::string &, optional< Function::DerivEvalType > >()));
    class_< GSLFDFSolver::Setup, bases< Solver::Setup > >("Setup",
      init< double, double >());
    class_< GSLFDFSolver::State, bases< NativeGradientSolver::State > >("State");
  }
#endif

#ifdef WITH_FORTRAN
  class_< LBFGSB, bases< NativeSolver > >("LBFGSB");
  //class_< LMBM, bases< Solver > >("LMBM");
#endif
  register_ptr_to_python< boost::shared_ptr< Solver::Results > >();
  register_ptr_to_python< boost::shared_ptr< Solver::Setup > >();
  register_ptr_to_python< boost::shared_ptr< NativeSolver::State > >();
}

#define SOLVERS_HPP

#endif
