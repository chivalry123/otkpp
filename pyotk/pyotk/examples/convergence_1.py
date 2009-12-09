# This example demonstrates comparing convergence 
# rates of different algorithms.

from pyotk.native import *
from pyotk.testproblems import *
from pyotk.convergence import *

# Initialize the list of algorithms.
S = [ LinminBFGS(LinminBFGS.LinminType.morethuente),
      LinminBFGS(LinminBFGS.LinminType.fletcher),
      ConjGradMT(ConjGradType.FR),
      ConjGradMT(ConjGradType.PR) ]

# Initialize the list of test problems.
P = [ Beale(),
      HelicalValley(),
      Wood(),
      ExtendedRosenbrock(n=10) ]

# Run each algorithm on each test problem 
# and draw convergence plots.
compare_convergence(S, P);
