
#include "numpy_utils.hpp"

#include <boost/python.hpp>
#include <otkpp/function/Function.h>

using namespace boost::python;
using namespace boost::python::numeric;
using namespace numpy_utils;

list tabulate_function(const Function &f, int nPoints,
                       const tuple &xRange, const tuple &yRange)
{
  int i, j;
  list result;
  vector< double > xy(2);
  
  array::set_module_and_type("numpy", "ndarray");
  
  array X = zeros(nPoints, nPoints);
  array Y = zeros(nPoints, nPoints);
  array FXY = zeros(nPoints, nPoints);
  
  double x1 = extract< double >(xRange[0]);
  double x2 = extract< double >(xRange[1]);
  double y1 = extract< double >(yRange[0]);
  double y2 = extract< double >(yRange[1]);
  
  for(i = 0; i < nPoints; i++)
  {
    for(j = 0; j < nPoints; j++)
    {
      double x = x1 + j * (x2 - x1) / (nPoints - 1);
      double y = y1 + i * (y2 - y1) / (nPoints - 1);
      X[make_tuple(i, j)] = x;
      Y[make_tuple(i, j)] = y;
      xy[0] = x;
      xy[1] = y;
      FXY[make_tuple(i, j)] = f(xy);
    }
  }
  
  result.append(X);
  result.append(Y);
  result.append(FXY);
  
  return result;
}

BOOST_PYTHON_MODULE(plotutils)
{
  def("tabulate_function", tabulate_function);
}
