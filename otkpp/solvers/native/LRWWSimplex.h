
#ifndef LRWWSIMPLEX_H

#include <otkpp/solvers/native/NativeSolver.h>

class LRWWSimplex : public NativeSolver
{
 public:
  class Simplex
  {
   public:
    enum OpType
    {
      EXPANSION,
      INSIDE_CONTRACTION,
      OUTSIDE_CONTRACTION,
      REFLECTION
    };
    
    Simplex() { }
    Simplex(int n);
    void computeCentroid();
    void computeExpansion(vector< double > &xe);
    void computeInsideContraction(vector< double > &xic);
    void computeOutsideContraction(vector< double > &xoc);
    void computeReflection(vector< double > &xr);
    const vector< double > &getCentroidPoint() const;
    double getfx(int i) const;
    const matrix_column< const matrix< double > > getx(int i) const;
    void improveHighestVertex(const vector< double > &x, double fx, OpType opType);
    void setVertex(int i, const vector< double > &x, double fx);
    void shrink(Function &f);
    void sortVertices();
   private:
    int n_;                // the number of vertices
    matrix< double > X_;   // (n+1)*n matrix of simplex vertices, one vertex per each matrix column
    vector< double > fx_;  // (n+1)-dimensional vector of function values at the vertices
    double vol_;           // simplex volume
    vector< double > xc_;  // the previously computed centroid point
  };
  
  struct State : public Cloneable< State, NativeSolver::State >
  {
    Simplex S;
  };
  
  unsigned int getM() const;
  std::string getName() const;
  const State &getState() const { return state_; }
  const matrix< double > getXArray() const;
  bool hasBuiltInStoppingCriterion() const { return false; }
  bool usesGradient() const;
  bool usesHessian() const;
 private:
  State state_;
  vector< double > xe_;
  vector< double > xic_;
  vector< double > xoc_;
  vector< double > xr_;
  
  NativeSolver::IterationStatus iterate_();
  void doSetup_(const Function &objFunc,
                const vector< double > &x0,
                const Solver::Setup &solverSetup,
                const Constraints &C);
};

#define LRWWSIMPLEX_H

#endif
