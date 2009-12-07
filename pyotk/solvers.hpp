
#ifndef SOLVERS_HPP

#include <otkpp/localsolvers/ExternalSolver.h>
#include <otkpp/localsolvers/native/ConjGradMT.h>
//#include <otkpp/localsolvers/native/DSLA.h>
#include <otkpp/localsolvers/native/DSQA.h>
#include <otkpp/localsolvers/native/HookeJeeves.h>
#include <otkpp/localsolvers/native/LinminBFGS.h>
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
      .def_readonly("m", &Solver::Setup::m)
      .def_readonly("n", &Solver::Setup::n)
      .def_readonly("objfunc", &Solver::Setup::objFunc);
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
      .def_readonly("f", &NativeSolver::State::f)
      .add_property("x", make_getter(&NativeSolver::State::x, return_value_policy< return_by_value >()))
      .add_property("X", make_getter(&NativeSolver::State::X, return_value_policy< return_by_value >()));
    class_< NativeSolver::Results, bases< Solver::Results > >("Results")
      .add_property("states", make_getter(&NativeSolver::Results::states, return_value_policy< return_by_value >()));
  }
  
  {
    scope gradientsolver_scope(class_< GradientSolver, boost::noncopyable >("GradientSolver", no_init));
    class_< GradientSolver::State, bases< NativeSolver::State >, boost::noncopyable >("State", no_init)
      .add_property("g", make_getter(&GradientSolver::State::g, return_value_policy< return_by_value >()));
  }
  
  enum_< ConjGradMT::Type >("ConjGradType")
    .value("FR", ConjGradMT::FLETCHER_REEVES)
    .value("PR", ConjGradMT::POLAK_RIBIERE);
  class_< ConjGradMT, bases< NativeSolver > >("ConjGradMT",
    init< ConjGradMT::Type >());
  
  {
    scope dsqa_scope(class_< DSQA, bases< NativeSolver > >("DSQA"));
    class_< DSQA::State, bases< NativeSolver::State > >("State")
      .def_readonly("delta", &DSQA::State::delta)
      .def_readonly("model", &DSQA::State::model);
  }
  
  //class_< DSLA, bases< NativeSolver > >("DSLA");
  class_< HookeJeeves, bases< NativeSolver > >("HookeJeeves");
  enum_< LinminBFGS::LinMinType >("BFGSLmType")
    .value("fletcher", LinminBFGS::FLETCHER)
    .value("morethuente", LinminBFGS::MORE_THUENTE);
  class_< LinminBFGS::Setup, bases< Solver::Setup > >("LinminBFGSSetup",
    init< optional< const LineMinimizer::Setup &, const matrix< double > & > >());
  class_< LinminBFGS, bases< NativeSolver > >("LinminBFGS",
    init< optional< LinminBFGS::LinMinType, int > >());
    class_< DoglegBFGS, bases< NativeSolver > >("DoglegBFGS");
    class_< PARTAN, bases< NativeSolver > >("PARTAN");
    class_< SteihaugSR1, bases< NativeSolver > >("SteihaugSR1");

  {
    scope lineminimizer_scope(class_< LineMinimizer, boost::noncopyable >("LineMinimizer", no_init));
    class_< LineMinimizer::Setup, boost::noncopyable >("Setup", no_init);
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
    scope gslfdfsolver_scope(class_< GSLFDFSolver, bases< NativeSolver > >("GSLfdfsolver",
        init< const std::string &, optional< Function::DerivEvalType > >()));
    class_< GSLFDFSolver::Setup, bases< Solver::Setup > >("Setup",
        init< double, double >());
    class_< GSLFDFSolver::State, bases< GradientSolver::State > >("State");
  }
#endif

#ifdef WITH_FORTRAN
  class_< LBFGSB, bases< NativeSolver > >("LBFGSB");
#endif
  register_ptr_to_python< boost::shared_ptr< Solver::Results > >();
  register_ptr_to_python< boost::shared_ptr< Solver::Setup > >();
  register_ptr_to_python< boost::shared_ptr< NativeSolver::State > >();
}

#define SOLVERS_HPP

#endif
