
#include "CompoundStoppingCriterion.h"
#include "ConjGradMT.h"
#include "DoglegBFGS.h"
#include "DSQA.h"
#include "FDistToMinTest.h"
#include "Function.h"
#include "GradNormTest.h"
#ifdef WITH_GSL
#include "GSLFSolver.h"
#include "GSLFDFSolver.h"
#endif
#include "HookeJeeves.h"
#include "LinminBFGS.h"
#include "LRWWSimplex.h"
#include "MaxNumIterTest.h"
#include "MGHTestFunction.h"
#include "MNewton.h"
#include "NativeSolver.h"
#include "SteihaugSR1.h"
#include "PARTAN.h"
#include "XDistToMinTest.h"
#ifdef WITH_FORTRAN
#include "LBFGSB.h"
#endif

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Tests

#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <list>
#include <string>

struct Fixture
{
  const std::string F_EXPR;
  const unsigned int MAX_NUM_ITER;
  
  std::list< NativeSolver * > solvers;
  CompoundStoppingCriterion *stopCrit;
  std::list< Function * > testFunctions;
  vector< double > xMin;
  
  //F_EXPR = "(1.5-x*(1-y))^2+(2.25-x*(1-y^2))^2+(2.625-x*(1-y^3))^2";
  //F_EXPR = "x*x+y*y";

  Fixture() : F_EXPR("(1-x)^2+100*(y-x*x)^2"), MAX_NUM_ITER(1000), 
    xMin(vector< double >(2))
  {
    initSolvers_();
    initTestFunctions_();
    xMin[0] = xMin[1] = 1.0;
    stopCrit = new CompoundStoppingCriterion(
      XDistToMinTest(xMin, 1e-6, false) + MaxNumIterTest(MAX_NUM_ITER));
  }
  
  ~Fixture()
  {
    freeSolvers_();
    freeTestFunctions_();
    delete stopCrit;
  }

  void freeSolvers_()
  {
    std::list< NativeSolver * >::iterator it;
    for(it = solvers.begin(); it != solvers.end(); it++)
      delete *it;
  }

  void freeTestFunctions_()
  {
    std::list< Function * >::iterator it;
    for(it = testFunctions.begin(); it != testFunctions.end(); it++)
      delete *it;
  }

  void initSolvers_()
  {
#ifdef WITH_GSL
    solvers.push_back(new GSLFSolver(gsl_multimin_fminimizer_nmsimplex));
    solvers.push_back(new GSLFDFSolver(gsl_multimin_fdfminimizer_conjugate_fr));
    solvers.push_back(new GSLFDFSolver(gsl_multimin_fdfminimizer_conjugate_pr));
    solvers.push_back(new GSLFDFSolver(gsl_multimin_fdfminimizer_vector_bfgs));
    solvers.push_back(new GSLFDFSolver(gsl_multimin_fdfminimizer_vector_bfgs2));
#endif
    solvers.push_back(new DSQA());
    solvers.push_back(new LRWWSimplex());
    solvers.push_back(new DoglegBFGS());
    solvers.push_back(new SteihaugSR1());
    solvers.push_back(new LinminBFGS(LinminBFGS::FLETCHER));
    solvers.push_back(new LinminBFGS(LinminBFGS::MORE_THUENTE));
    solvers.push_back(new LinminBFGS(LinminBFGS::MORE_THUENTE, 8));
    solvers.push_back(new MNewton());
    solvers.push_back(new PARTAN());
    solvers.push_back(new HookeJeeves());
    solvers.push_back(new ConjGradMT(ConjGradMT::FLETCHER_REEVES));
    solvers.push_back(new ConjGradMT(ConjGradMT::POLAK_RIBIERE));
#ifdef WITH_FORTRAN
    solvers.push_back(new LBFGSB());
#endif
  }

  void initTestFunctions_()
  {
#ifdef WITH_LIBMATHEVAL
    testFunctions.push_back(new Function(F_EXPR, Function::DERIV_SYMBOLIC));
#endif
    testFunctions.push_back(new ExtendedRosenbrock(2, Function::DERIV_FDIFF_CENTRAL_2));
  }

};

/*static void printResults(const std::string &algoName, 
                         int n_iter, 
                         const vector< double > &x, 
                         double f,
                         NativeSolver::IterationStatus status)
{
  if (status == NativeSolver::ITERATION_CONTINUE || 
      status == NativeSolver::ITERATION_SUCCESS)
    printf("%-15s: niter: %-4d x*=%.5f  y*=%.5f  f(x*,y*)=%.5e\n",
           algoName.c_str(), n_iter, x[0], x[1], f);
  else
    printf("%-15s: FAILURE (%s)\n", algoName.c_str(), "???"); // TODO: error code string
}*/

BOOST_FIXTURE_TEST_SUITE(TestSolvers, Fixture)

BOOST_AUTO_TEST_CASE(testConvergence)
{
  vector< double > x0(2);
  std::list< Function * >::const_iterator f;
  std::list< NativeSolver * >::const_iterator s;
  NativeSolver::IterationStatus status;
  int k;
  
  x0[0] = -1.2;
  x0[1] = 1.0;
  
  int fi = 0;
  for(f = testFunctions.begin(); f != testFunctions.end(); f++)
  {
/*#ifdef WITH_LIBMATHEVAL
    if (fi == 0)
      std::cout<<"Rosenbrock function with symbolic evaluation:"<<std::endl;
    else
#endif
      std::cout<<"Precompiled Rosenbrock function with finite-difference derivatives:"
               <<std::endl;*/
    
    for(s = solvers.begin(); s != solvers.end(); s++)
    {
      Solver::Results results = *(*s)->solve(**f, x0, *stopCrit);
      BOOST_CHECK_MESSAGE(norm_2(results.xMin - xMin) < 1e-6, 
                          "solver " + (*s)->getName() + " failed to converge");
      
      /*(*s)->setup(**f, x0);
      
      k = 0;
      status = NativeSolver::ITERATION_CONTINUE;
      do
      {
        status = (*s)->iterate();
        k++;
      }
      while(status == NativeSolver::ITERATION_CONTINUE && 
            ((*s)->hasBuiltInStoppingCriterion() || stopCrit->test(**s) == false));
      
      BOOST_CHECK_MESSAGE(norm_2((*s)->getX() - xMin) < 1e-6, 
                          "solver " + (*s)->getName() + " failed to converge");
      
      printResults((*s)->getName(), k, (*s)->getX(), 
                  (*s)->getFVal(), status);*/
    }
    
    fi++;
  }
}

BOOST_AUTO_TEST_SUITE_END()

// experimental code

/*#ifdef WITH_FORTRAN
static void testLBFGSB()
{
  LBFGSB solver;
  
  Function f(F_EXPR);
  vector< double > x0(2);
  x0[0] = -1.2;
  x0[1] = 1.0;
  
  solver.setup(f, x0);
  for(int i = 0; i < 100; i++)
    solver.iterate();
}

static void testLMBM()
{
  LMBM solver;
  
  Function f(F_EXPR);
  vector< double > x0(2);
  x0[0] = -1.2;
  x0[1] = 1.0;
  
  solver.solve(f, x0);
}*/

/*static void testNEWUOA()
{
  NEWUOA solver;
  
  //Gulf f(10, Function::DERIV_FDIFF_CENTRAL_2);
  ExtendedRosenbrock f(50, Function::DERIV_FDIFF_CENTRAL_2);
  vector< double > x0(50);
  for(int i = 0; i < 50; i++)
  {
    if(i % 2 == 0)
      x0[i] = -1.2;
    else
      x0[i] = 1.0;
  }
  /*vector< double > x0(3);
  x0[0] = 5;
  x0[1] = 2.5;
  x0[2] = 0.15;*/
  /*solver.solve(f, x0, MaxNumIterTest(10000));
}*/
//#endif
