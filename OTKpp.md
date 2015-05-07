**This wiki page is outdated. A new page will be written for OTK++/PyOTK versions 0.3, which will be released soon.**

# Examples #

The following code example demonstrates initializing the objective function, solver, starting point and stopping criterion and iterating the algorithm until the stopping criterion is satisfied.

```
#include <otkpp/config.h>
#include <otkpp/function/Function.h>
#include <otkpp/localsolvers/native/LinminBFGS.h>
#include <otkpp/stopcrit/GradNormTest.h>

int main()
{
  Function f("(1-x)^2+100*(y-x*x)^2");
  LinminBFGS s(LinminBFGS::MORE_THUENTE);
  vector< double > x0(2);
  GradNormTest stopCrit(1e-8);
  NativeSolver::IterationStatus status;
  
  x0[0] = -1.2;
  x0[1] = 1.0;
  s.setup(f, x0);
  
  do {
    status = s.iterate();
  }
  while(status == NativeSolver::ITERATION_CONTINUE && 
        stopCrit.test(s) == false);
  
  printf("%-10s: niter: %-4d x*=%.5f  y*=%.5f  f(x*,y*)=%.5e\n", 
         s.getName().c_str(), s.getNumIter(), s.getX()[0], s.getX()[1], s.getFVal());
  
  return EXIT_SUCCESS;
}
```

In the above example, the objective function is initialized with the symbolic expression of the Rosenbrock function. By default, automatic differentiation is used for symbolic function expressions:

```
Function f("(1-x)^2+100*(y-x*x)^2");
```

The algorithm is BFGS with the More and Thuente line search:

```
LinminBFGS s(LinminBFGS::MORE_THUENTE);
```


The stopping criterion is initialized by constructing a gradient norm criterion with threshold value 1e-8:

```
GradNormTest stopCrit(1e-8);
```

Each native solver implements the methods setup and iterate. The setup method initializes the solver with the given function and starting point:

```
s.setup(f, x0);
```

Iterate takes one iteration step. After each iteration step, the state of the algorithm(current iterate, function value,...) can be queried, and the stopping criterion can be tested. In this case, we are interested in the status of the iteration after each step:

```
do {
  status = s.iterate();
}
while(status == NativeSolver::ITERATION_CONTINUE && 
      stopCrit->test(s) == false);
```

After the algorithm has terminated, its state is queried, and the results are printed. The number of used iterations, final x and y value and final function value are queried with methods 'getNumIter', 'getX' and 'getFVal', respectively:

```
printf("%-10s: niter: %-4d x*=%.5f  y*=%.5f  f(x*,y*)=%.5e\n", 
         s.getName().c_str(), s.getNumIter(), s.getX()[0], s.getX()[1], s.getFVal());
```

These quantities can also be queried at any intermediate iteration step. Also, these methods are implemented for all native solvers implemented in OTK++.

Download example: [otkpp\_example.cpp](http://otkpp.googlecode.com/svn/site/examples/otkpp/otkpp_example.cpp)

The example compiles with the following command:

```
g++ otkpp_example.cpp -lotkpp -lmatheval -lgsl -lgslcblas -o otkpp_example
```