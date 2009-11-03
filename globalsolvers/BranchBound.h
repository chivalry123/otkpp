
#ifndef BRANCHBOUND_H

#include <otkpp/globalsolvers/GlobalSolver.h>

#include <list>
#include <vector>

class BranchBound : public GlobalSolver
{
 public:
 private:
  struct Cell
  {
    int n;
    std::vector< double > L;
    std::vector< double > U;
    
    Cell(int n);
  };
  
  void iterate_();
  static void subdivideCell_(const Cell &cell, std::list< Cell > &subCells);
};

#define BRANCHBOUND_H

#endif
