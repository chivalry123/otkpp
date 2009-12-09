# This example demonstrates invoking the GSL simplex algorithm 
# and plotting the generated iterates and simplices.

from pyotk.native import *
from pyotk.plot2d import *
from pyotk.testproblems import *

# Initialize the Penalty Function II test problem with 
# two variables and its plotting ranges.
p = PenaltyFunctionII(n=2)
x1 = -0.9
x2 = 0.75
y1 = -0.75
y2 = 1.75
z1 = p.plot_spec.z_range[0]
z2 = p.plot_spec.z_range[1]
z_logscale = p.plot_spec.z_logscale

# Initialize the GSL simplex algorithm with default parameters.
s = LRWWSimplex()

# Call the driver routine and plot results.
x0 = (-0.8, 0.5)
results = s.solve(p.f, x0, MaxNumIterTest(100), Solver.DefaultSetup(), NoConstraints(), False)
plot_iterates_on_contours(results, x1, x2, y1, y2, z1, z2)
