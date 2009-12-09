from pyotk.io import *
from pyotk.native import *
from pyotk.testproblems import *

s = LinminBFGS(LinminBFGS.LinminType.morethuente)
p = ExtendedRosenbrock(n=2)

x0 = (-1.2, 1)
sc = p.stopcrit

results = s.solve(p.f, x0, sc, Solver.DefaultSetup(), NoConstraints(), False)
print_results(results, 2)
