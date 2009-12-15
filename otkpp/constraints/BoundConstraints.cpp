
#include "BoundConstraints.h"

BoundConstraints::BoundConstraints(int n)
{
  L.resize(n);
  U.resize(n);
  types.resize(n);
  for(int i = 0; i < n; i++)
    types[i] = BoundConstraints::NONE;
}

bool BoundConstraints::isFeasible(const vector< double > &x) const
{
  return false; // TODO
}
