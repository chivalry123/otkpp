# Implemented algorithms #

OTK++ currently implements only a limited set of algorithms, but more will follow in future releases. The following basic algorithms are currently implemented for unconstrained optimization:

**Direct search:**
  * Hooke-Jeeves
  * Nelder-Mead simplex
  * DSQA (a derivative-free algorithm with quadratic interpolation)
**Gradient-based algorithms with line searches:**
  * PARTAN
  * Fletcher-Reeves and Polak-Ribiere conjugate gradient
  * BFGS and L-BFGS
  * Modified Newton
**Gradient-based algorithms with trust region approach:**
  * Steihaug-SR1
  * Dogleg-BFGS

Due to the modular design of OTK++, many components of the above algorithms such as line search algorithms or trust region minimizers are interchangeable. These subcomponents currently implemented in OTK++ include:

**Line search algorithms:**
  * More and Thuente
  * Fletcher
**Trust region minimizers:**
  * Steihaug
  * Dogleg

OTK++ also wraps existing C and Fortran implementations by other authors:

  * Algorithms implemented in the GSL multimin package (version 1.11)
  * L-BFGS-B for bound-constrained large-scale optimization `[3]`
  * LMBM (limited memory bundle method) for nonsmooth large-scale optimization `[4]`


---


# References #

The algorithms implemented in OTK++ are based
on the following references:

```
[1] J. Nocedal and S.J. Wright, Numerical Optimization, 1999
[2] C.T. Kelley, Iterative Methods for Optimization, 1999
[3] C. Zhu, R.H. Byrd and J. Nocedal, L-BFGS-B: Algorithm 778: L-BFGS-B, 
    FORTRAN routines for large scale bound constrained optimization (1997), 
    ACM Transactions on Mathematical Software, Vol 23, Num. 4, pp. 550-560
[4] N. Karmitsa,  "LMBM - FORTRAN Subroutines for Large-Scale Nonsmooth 
    Minimization: User's Manual", TUCS Technical Report, No. 856, Turku 
    Centre for Computer Science, Turku, 2007.
```