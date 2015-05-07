# Accessing the OTK++ interfaces from Python #

The key idea of the PyOTK is to implement seamless interface for the OTK++ classes. That is, the syntax for calling the OTK++ classes and methods from the Python side has as identical syntax with the corresponding C++ code as possible. This can be very conveniently done with Boost.Python. We begin with the following example that demonstrates invoking the OTK++ solvers from the Python side.

First we need to import the necessary PyOTK modules. The facilities for using the OTK++ classes and methods from Python are implemented in the 'native' module:

```
>>> from pyotk.native import *
```

The 'testproblems' module implements some of the test problems given by More, Garbow and Hillstrom (1981):

```
>>> from pyotk.testproblems import *
```

In this context, a "test problem" means a test function associated with additional information such as the default starting point, stopping criterion and preferred plotting ranges. Most test functions have a symbolic expression, and some arbitrary-dimensional test problems also implement generators for their symbolic expressions. See the source code of this module for details.

And the 'io' module implements methods for printing the results of a solver:

```
from pyotk.io import *
```

The Python methods for instantiating OTK++ object have a similar syntax with their C++ counterparts. For example, the 'LinminBFGS' solver with the More and Thuente line search and an the two-dimensional Rosenbrock test problem from the 'testproblems' module can be instantiated as follows:

```
>>> s = s = LinminBFGS(LinminBFGS.LinminType.morethuente)
>>> p = ExtendedRosenbrock(n=2)
```

The starting point is specified as a tuple, and the stopping criterion can be instantiated with constructors similar to those in the C++ library. In this case we test the gradient norm against the given threshold value:

```
>>> x0 = (-1.2, 1)
>>> sc = GradNormTest(eps=1e-8)
```

As in the C++ library, each solver implements the 'solve' method for solving the given test problem. Its parameters are the objective function, starting point, stopping criterion, solver parameters and constraints:

```
>>> results = s.solve(p.f, x0, sc, Solver.DefaultSetup(), NoConstraints(), False)
```

The actual function associated with the test problem is accessed with the 'f' attribute. Each algorithm implemented in OTK++ has some good-known choice of default parameter values. By specifying 'Solver.DefaultSetup()', we choose to use them. If necessary, an instance of a class holding algorithm-specific parameters can also be used. See the reference manual for more details. We also choose not to impose any constraints by specifying 'NoConstraints()' for the constraints argument. The last argument can be used to enable a special time-test mode for measuring performance. In this mode, storing any intermediate values or solver states is skipped, since it could affect the measurements.

A summary of the results can be printed by using the 'print\_results' method implemented in the 'io' module:

```
>>> print_results(results, 2)
```

This method prints the following output with a list of each iterate and its corresponding function value and the final value on termination of the algorithm:

```
k      x1         x2         f(x1,x2)
0      -1.200     1.000      24.2
1      -1.030     1.069      4.1282
2      -0.931     0.830      3.8589
3      -0.789     0.565      3.5308
4      -0.357     0.086      2.0182
5      -0.372     0.133      1.8844
6      -0.181     -0.002     1.5153
7      0.006      -0.043     1.1732
8      0.033      0.007      0.93746
9      0.176      0.008      0.72981
10     0.259      0.034      0.65748
11     0.578      0.321      0.19539
12     0.572      0.324      0.18433
13     0.692      0.464      0.11567
14     0.837      0.690      0.038802
15     0.833      0.693      0.028063
16     0.913      0.827      0.011232
17     0.989      0.979      0.00036423
18     0.997      0.993      1.2821e-05
19     1.000      1.000      2.9413e-07
20     1.000      1.000      4.2333e-10
21     1.000      1.000      3.4343e-13
22     1.000      1.000      1.0598e-20

Minimizer:                (1,1)
Minimum value:            1.0598e-20
Iterations:               23
Function evaluations:     64
```

In addition to the above output, the results are also stored in the returned 'results' object. Depending on the used solver, it contains at least the following attributes:

| Attribute | Description |
|:----------|:------------|
| converged | was the stopping criterion satisfied on termination |
| f\_min | the final function value |
| num\_func\_eval | number of function evaluations |
| num\_grad\_eval | number of gradient evaluations |
| num\_iter | number of iterations |
| setup | used input parameters |
| term\_val | the final value that was tested against the stopping criterion |
| time | the used time, if it was measured |
| x\_min | the final iterate |

Some algorithms also support storing the state of the algorithm after each iteration step. These states are stored in the 'states' attribute.

Download example: [solver\_call.py](http://otkpp.googlecode.com/svn/site/examples/pyotk/solver_call.py)

# Visualization #

PyOTK uses Matplotlib for plotting. The following visualizations for analyzing the behaviour of minimization algorithms are currently implemented:

  * plotting the iterates on top of the contours of the objective function
  * convergence plots
  * the Dolan and More performance profiles
  * plotting basin sets (the sets containing points from which the iteration converges to the given minimizer)
  * the effect of problem scaling on convergence rates

## Contour and iterate plots (pyotk.plot2d) ##

The 'plot2d' module implements methods for visualizing the iteration of a solver. For a given function of two variables, the iterates produced by a solver can be plotted on top of contours of the objective function.

The following example demonstrates minimizing the Beale test function and plotting the results using the 'plot\_iterates\_on\_contours' method.

```
from pyotk.native import *
from pyotk.plot2d import *
from pyotk.testproblems import *

p = Beale()
x1 = p.plot_spec.x_range[0]
x2 = p.plot_spec.x_range[1]
y1 = p.plot_spec.y_range[0]
y2 = p.plot_spec.y_range[1]
z1 = p.plot_spec.z_range[0]
z2 = p.plot_spec.z_range[1]

s = LinminBFGS()

results = s.solve(p.f, p.x0, p.stopcrit, Solver.DefaultSetup(), NoConstraints(), False)
plot_iterates_on_contours(results, x1, x2, y1, y2, z1, z2)
```

Download example: [plot2d\_example\_1.py](http://otkpp.googlecode.com/svn/site/examples/pyotk/plot2d_example_1.py)

## Convergence plots(pyotk.convergence) ##

A convergence plot represents the given quantity (e.g. distance to an a-priori known minimizer) as a function of number of used iterations. The 'convergence' module implements methods drawing convergence plots of one or more algorithms into the same figure.

The first example demonstrates using the 'compare\_convergence' method for comparing convergence rates of different algorithms.

```
from pyotk.native import *
from pyotk.testproblems import *
from pyotk.convergence import *

S = [ LinminBFGS(LinminBFGS.LinminType.morethuente),
      LinminBFGS(LinminBFGS.LinminType.fletcher),
      ConjGradMT(ConjGradType.FR),
      ConjGradMT(ConjGradType.PR) ]

P = [ Beale(),
      HelicalValley(),
      Wood(),
      ExtendedRosenbrock(n=10) ]

compare_convergence(S, P);
```

The second example demonstrates using the 'convergence\_plot' method for plotting gradient norm and objective function value as a function of used iterations.

```
from pyotk.native import *
from pyotk.testproblems import *
from pyotk.convergence import *

p = ExtendedRosenbrock(8)
results = LinminBFGS().solve(p.f, p.x0, GradNormTest(eps=1e-10),
  Solver.DefaultSetup(), NoConstraints(), False)

quantities = ['gradnorm', 'funcval']
convergence_plot(results, quantities)
```

Download example 1: [convergence\_example\_1.py](http://otkpp.googlecode.com/svn/site/examples/pyotk/convergence_example_1.py)

Download example 2: [convergence\_example\_2.py](http://otkpp.googlecode.com/svn/site/examples/pyotk/convergence_example_2.py)

## Performance profiles (pyotk.perfprof) ##

The Dolan and More performance profiles are commonly used for comparing performance of different algorithms. For a given set of test problems and a given set of solvers, these performance profiles can be computed by using the 'perfprof' module.

The following example demonstrates using the 'performance\_profile' method for computing performance profiles. The list S contains the solvers to use, and the list P contains the set of test problems.

```
from pyotk.native import *
from pyotk.testproblems import *
from pyotk.perfprof import *

S = [ ConjGradMT(ConjGradType.FR),
      ConjGradMT(ConjGradType.PR),
      SteihaugSR1(),
      LinminBFGS(LinminBFGS.LinminType.morethuente) ]

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

performance_profile(S, P, True)
```

Download example: [perfprof\_example.py](http://otkpp.googlecode.com/svn/site/examples/pyotk/perfprof_example.py)

<img src='http://otkpp.googlecode.com/svn/site/images/contour_with_iterates.png'>
<img src='http://otkpp.googlecode.com/svn/site/images/convergence_plot.png'>
<img src='http://otkpp.googlecode.com/svn/site/images/perfprof.png'>

<h2>Basin sets (pyotk.startingpoint)</h2>

Most minimization algorithms are more or less sensitive to the choice of starting point. A term "basin set" is commonly used for a set that contains the starting points from which the iteration converges to the given minimizer. The following example demonstrates how to use PyOTK for plotting these basin sets for a multimodal function with known minimizers. The 'starting_point_analysis' method called in the following example basically loops through each starting point in the given rectangular grid and classifies each of these starting points according to the minimizer the iteration converges to.<br>
<br>
<pre><code>from pyotk.startingpoint import *<br>
<br>
f = Function("(x^2+y-11)^2+(x+y^2-7)^2")<br>
<br>
p_min = [ (3, 2),<br>
          (-2.805118, 3.131312),<br>
          (-3.779310, -3.283186),<br>
          (3.584428, -1.848126) ]<br>
<br>
s = LinminBFGS(LinminBFGS.LinminType.morethuente)<br>
starting_point_analysis(s, f, p_min, -5, 5, -5, 5, 150)<br>
s = SteihaugSR1()<br>
starting_point_analysis(s, f, p_min, -5, 5, -5, 5, 150)<br>
</code></pre>

<img src='http://otkpp.googlecode.com/svn/site/images/basin_sets.png'>