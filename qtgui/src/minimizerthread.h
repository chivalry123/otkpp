
#ifndef MINIMIZERTHREAD_H

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <QThread>
#include <string>

using namespace boost::numeric::ublas;

class BoundConstraints;
class Function;
class Solver;
class StoppingCriterion;

class MinimizerThread : public QThread
{
 public:
  struct Results
  {
    double fMin;
    std::list< vector< double > > iterates;
    std::list< matrix< double > > iteratePointSets;
    unsigned int numIter;
    unsigned int numFuncEval;
    unsigned int numGradEval;
    double xMin, yMin;
  };
  
  MinimizerThread(Function *objFunc,
                  Solver *solver,
                  double x0,
                  double y0,
                  const StoppingCriterion *stopCrit,
                  const BoundConstraints *C);
  
  ~MinimizerThread();
  
  const Results &getResults() const;
  const Solver &getSolver();
  void raiseTerminationFlag();
  void run();
 private:
  const std::string algoName_;
  const BoundConstraints *C_;
  Function *objFunc_;
  Results results_;
  Solver *solver_;
  const StoppingCriterion *stopCrit_;
  bool terminationFlag_;
  double x0_;
  double y0_;
};

#define MINIMIZERTHREAD_H

#endif
