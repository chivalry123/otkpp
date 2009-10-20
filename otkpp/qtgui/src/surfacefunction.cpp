
#include "surfacefunction.h"

#include <Function.h>

#include <boost/numeric/ublas/vector.hpp>
#include <stdexcept>

using namespace boost::numeric::ublas;

static const Function *f_; // TODO: move this out of here!

SurfaceFunction::SurfaceFunction(Qwt3D::SurfacePlot *pw) : Qwt3D::Function(pw)
{
  f_ = NULL;
  zScaleType_ = Qwt3D::LINEARSCALE;
}

double SurfaceFunction::operator()(double x, double y)
{
  if(f_ != NULL)
  {
    double xScaled = x * (xMax_ - xMin_) + xMin_;
    double yScaled = y * (yMax_ - yMin_) + yMin_;
    vector< double > xv(2);
    xv(0) = xScaled;
    xv(1) = yScaled;
    double fxy = (*f_)(xv);
    if(fxy < zMin_ || fxy > zMax_)
      return NAN;
    if(zScaleType_ == Qwt3D::LOG10SCALE)
      return log10(fxy) / 8.0;
    else if(zScaleType_ == Qwt3D::LINEARSCALE)
      return (fxy - zMin_) / (zMax_ - zMin_);
  }
  else
    return NAN;
}

void SurfaceFunction_setFunction(const Function *f)
{
  f_ = f;
}

void SurfaceFunction::setDomain(double minu, double maxu, double minv, double maxv)
{
  Qwt3D::Function::setDomain(0.0, 1.0, 0.0, 1.0);
  xMin_ = minu;
  xMax_ = maxu;
  yMin_ = minv;
  yMax_ = maxv;
}

void SurfaceFunction::setMinZ(double val)
{
  if(zScaleType_ == Qwt3D::LINEARSCALE)
    Qwt3D::Function::setMinZ(0.0);
  else if(zScaleType_ == Qwt3D::LOG10SCALE)
    Qwt3D::Function::setMinZ(log10(val));
  zMin_ = val;
}

void SurfaceFunction::setMaxZ(double val)
{
  if(zScaleType_ == Qwt3D::LINEARSCALE)
    Qwt3D::Function::setMaxZ(1.0);
  else if(zScaleType_ == Qwt3D::LOG10SCALE)
    Qwt3D::Function::setMaxZ(log10(val));
  zMax_ = val;
}

void SurfaceFunction::setZAxisScaleType(Qwt3D::SCALETYPE scaleType)
{
  if(scaleType != Qwt3D::LOG10SCALE && 
     scaleType != Qwt3D::LINEARSCALE)
    throw std::runtime_error("Invalid scale type");
  
  zScaleType_ = scaleType;
  setMinZ(zMin_);
  setMaxZ(zMax_);
}
