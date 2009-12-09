# This example demonstrates computing the Dolan and More 
# performance profile of the given algorithms on the 
# given set of test problems.

from pyotk.native import *
from pyotk.testproblems import *
from pyotk.perfprof import *

# Initialize the list of algorithms.
S = [ ConjGradMT(ConjGradType.FR),
      ConjGradMT(ConjGradType.PR),
      SteihaugSR1(),
      LinminBFGS(LinminBFGS.LinminType.morethuente) ]

# Initialize the list of test problems.
P = [ PowellBadlyScaled(),
      BrownBadlyScaled(),
      Beale(),
      HelicalValley(),
      Gaussian(),
      Box(m=5),
      Wood(),
      BrownDennis(m=20),
      BiggsEXP6(m=13),
      Watson(n=6),
      ExtendedRosenbrock(n=10),
      ExtendedPowellSingular(n=12),
      PenaltyFunctionI(n=10),
      PenaltyFunctionII(n=10),
      VariablyDimensioned(n=10),
      Trigonometric(n=5),
      ChebyQuad(n=8, m=8) ]

# Compute the performance profile.
performance_profile(S, P, True)
