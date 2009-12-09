# This example demonstrates how scaling of objective 
# function parameters affects convergence rates.

from pyotk.scaling import *

# Plot scaling analysis graphs for the line search 
# BFGS and Polak-Ribiere conjugate gradient algorithms.
s = LinminBFGS(LinminBFGS.LinminType.morethuente)
scaling_analysis(s)
s = ConjGradMT(ConjGradType.PR)
scaling_analysis(s)
