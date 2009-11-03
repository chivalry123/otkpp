
#include "BranchBound.h"

void BranchBound::iterate_()
{
  
}

void BranchBound::subdivideCell_(const Cell &cell, std::list< Cell > &subCells)
{
  Cell subCell(cell.n);
  
  subCells.clear();
  
  for(int i = 0; i < pow(2, cell.n); i++)
  {
    
    subCells.push_back(subCell);
  }
}
