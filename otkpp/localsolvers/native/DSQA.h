
#ifndef DSQA_H

#include <otkpp/localsolvers/native/NativeSolver.h>

/// A direct search algorithm based on Powell's NEWUOA.
class DSQA : public NativeSolver
{
 public:
  bool hasBuiltInStoppingCriterion() const;
  bool isGSLSolver() const;
 private:
  void generateInitialPoints_(const vector< double > &x0);
};

#define DSQA_H

#endif
