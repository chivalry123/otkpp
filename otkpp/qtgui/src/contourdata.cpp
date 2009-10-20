
#include "contourdata.h"

#include "Function.h"

#include <boost/numeric/ublas/vector.hpp>
#include <math.h>

using namespace boost::numeric::ublas;

ContourData::ContourData()
{
  f_ = NULL;
}

ContourData *ContourData::copy() const
{
  return new ContourData(*this);
}

QwtDoubleInterval ContourData::range() const
{
  return QwtDoubleInterval(valMin_, valMax_);
}

void ContourData::setFunction(Function *f)
{
  f_ = f;
}

void ContourData::setValueRange(double valMin, double valMax)
{
  valMin_ = valMin;
  valMax_ = valMax;
}

double ContourData::value(double x, double y) const
{
  if(f_ != NULL)
  {
    vector< double > xv(2);
    xv(0) = x;
    xv(1) = y;
    f_->disableEvalCounting();
    double v = (*f_)(xv);
    f_->enableEvalCounting();
    return v;
  }
  else
    return NAN;
}
