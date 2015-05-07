# Summary #

OTK++ (Optimization Toolkit) is a nonlinear optimization library written in C++. It also has a Python-based interface for testing and comparing algorithms and a Qt-based GUI for demonstration purposes. OTK++ relies to a large extent on the Boost libraries: linear algebra operations and Python interfaces are written by using uBLAS and Boost.Python.

## Introduction ##

The algorithms implemented in OTK++ are designed for finding local minima of real-valued multivariate functions. The aim is to implement algorithms especially for nonsmooth and constrained large-scale problems. The currently implemented algorithms are for solving unconstrained or box-constrained optimization problems of the form

<img src='http://otkpp.googlecode.com/svn/site/images/problem_defn.png'>

See the wiki pages for installation instructions and tutorials on how to use the library and its user interfaces.<br>
<br>
<b>Author:</b>

Seppo Pulkkinen <seppul@utu.fi>