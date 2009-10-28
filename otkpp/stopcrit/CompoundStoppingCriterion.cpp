
#include "CompoundStoppingCriterion.h"

#include <stdexcept>

CompoundStoppingCriterion CompoundStoppingCriterion::operator+(const StoppingCriterion &sc)
{
  std::list< const StoppingCriterion * >::const_iterator it;
  CompoundStoppingCriterion result(*this);
  
  result.stopCrit_.push_back(sc.clone());
  
  return result;
}

double CompoundStoppingCriterion::getTestValue(const NativeSolver &s) const
{
  throw std::runtime_error("operation not implemented");
}

bool CompoundStoppingCriterion::test(const NativeSolver &s) const
{
  std::list< const StoppingCriterion * >::const_iterator it;
  for(it = stopCrit_.begin(); it != stopCrit_.end(); it++)
  {
    if((*it)->test(s))
      return true;
  }
  
  return false;
}
