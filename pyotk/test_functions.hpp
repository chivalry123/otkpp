
#include <otkpp/testproblems/MGHTestFunction.h>

#include <boost/python.hpp>

void init_test_functions()
{
  class_< PowellBadlyScaled, bases< Function > >(
    "PowellBadlyScaled", init< Function::DerivEvalType >());
  class_< BrownBadlyScaled, bases< Function > >(
    "BrownBadlyScaled", init< Function::DerivEvalType >());
  class_< Beale, bases< Function > >(
    "Beale", init< Function::DerivEvalType >());
  class_< HelicalValley, bases< Function > >(
    "HelicalValley", init< Function::DerivEvalType >());
  class_< Gaussian, bases< Function > >(
    "Gaussian", init< Function::DerivEvalType >());
  class_< ExtendedRosenbrock, bases< Function > >(
    "ExtendedRosenbrock", init< int, Function::DerivEvalType >());
  class_< Gulf, bases< Function > >(
    "Gulf", init< int, Function::DerivEvalType >());
  class_< Box, bases< Function > >(
    "Box", init< int, Function::DerivEvalType >());
  class_< Wood, bases< Function > >(
    "Wood", init< Function::DerivEvalType >());
  class_< BrownDennis, bases< Function > >(
    "BrownDennis", init< int, Function::DerivEvalType >());
  class_< BiggsEXP6, bases< Function > >(
    "BiggsEXP6", init< int, Function::DerivEvalType >());
  class_< Watson, bases< Function > >(
    "Watson", init< int, Function::DerivEvalType >());
  class_< ExtendedPowellSingular, bases< Function > >(
    "ExtendedPowellSingular", init< int, Function::DerivEvalType >());
  class_< PenaltyFunctionI, bases< Function > >(
    "PenaltyFunctionI", init< int, Function::DerivEvalType >());
  class_< PenaltyFunctionII, bases< Function > >(
    "PenaltyFunctionII", init< int, Function::DerivEvalType >());
  class_< VariablyDimensioned, bases< Function > >(
    "VariablyDimensioned", init< int, Function::DerivEvalType >());
  class_< Trigonometric, bases< Function > >(
    "Trigonometric", init< int, Function::DerivEvalType >());
  class_< ChebyQuad, bases< Function > >(
    "ChebyQuad", init< int, int, Function::DerivEvalType >());
}
