# This example demonstrates plotting different quantities 
# as a function of used iteration steps.

from pyotk.native import *
from pyotk.testproblems import *
from pyotk.convergence import *

# Initialize the extended Rosenbrock function with 8 variables.
p = ExtendedRosenbrock(8)
# Start iteration from the default starting point 
# and use the BFGS algorithm with line searches.
results = LinminBFGS().solve(p.f, p.x0, GradNormTest(eps=1e-10),
  Solver.DefaultSetup(), NoConstraints(), False)

# Choose gradient norm and function value and plot them.
quantities = ['gradnorm', 'funcval']
convergence_plot(results, quantities)
