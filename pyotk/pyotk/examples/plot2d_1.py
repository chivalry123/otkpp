# This is a minimal example for invoking the driver routine 
# and plotting the results.

from pyotk.native import *
from pyotk.plot2d import *
from pyotk.testproblems import *

# Initialize the Beale test function and plotting ranges.
p = Beale()
x1 = p.plot_spec.x_range[0]
x2 = p.plot_spec.x_range[1]
y1 = p.plot_spec.y_range[0]
y2 = p.plot_spec.y_range[1]
z1 = p.plot_spec.z_range[0]
z2 = p.plot_spec.z_range[1]

# Initialize the line search BFGS algorithm with default parameters.
s = LinminBFGS()

# Call the driver routine and plot results.
results = s.solve(p.f, p.x0, p.stopcrit, Solver.DefaultSetup(), NoConstraints(), False)
plot_iterates_on_contours(results, x1, x2, y1, y2, z1, z2)
