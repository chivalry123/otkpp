# This example demonstrates how the minimizer the 
# iteration converges to depends on the choice of 
# starting point.

from pyotk.startingpoint import *

# Initialize the Himmelblau function 
# (a test function with multiple local minima).
f = Function("(x^2+y-11)^2+(x+y^2-7)^2")

# It has the following minimizers:
p_min = [ (3, 2),
          (-2.805118, 3.131312),
          (-3.779310, -3.283186),
          (3.584428, -1.848126) ]

# Do starting point analysis for the line search 
# BFGS algoritm and the trust region-based Steihaug 
# SR1 algorithm.
s = LinminBFGS(LinminBFGS.LinminType.morethuente)
starting_point_analysis(s, f, p_min, -5, 5, -5, 5, 150)
s = SteihaugSR1()
starting_point_analysis(s, f, p_min, -5, 5, -5, 5, 150)
