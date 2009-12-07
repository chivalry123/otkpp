"""Implements tools for printing result tables.

This module implements functions for printing statistical 
information such as number of iterations, function iterations, 
etc. about iteration results.
"""

from native import *
import sys
from testproblems import *

def print_table(T, num_rows, num_cols, column_widths, stream=None, print_mode=None):
	tr = 0
	
	w = 0
	for ci in range(num_cols):
		w += column_widths[ci] + 1
		
	for ri in range(2*num_rows + 1):
		if ri % 2 == 0:
			for i in range(w+1):
				sys.stdout.write('-')
			print
		else:
			sys.stdout.write('|')
			for ci in range(num_cols):
				format_str = "%-" + str(column_widths[ci])
				if type(T[tr][ci]) == str:
					format_str += "s"
				elif type(T[tr][ci]) == float:
					format_str += '.3f'
				else:
					format_str += 'd'
				print format_str % T[tr][ci],
				sys.stdout.write('|')
			print
		
		if ri % 2 == 1:
			tr = tr + 1

def solver_results_table(S, f, x0, sc, max_num_iter):
	"""For a given set of solvers S, the given test function f, 
print a table of statistical information of the given solvers. 
The starting point and stopping criterion are specified with 
the parameters x0 and sc, respectively. Iterations are terminates 
after max_num_iter steps. The results table contains the following 
information for each solver on termination of the iteration:
  - number of iterations
  - number of function evaluations
  - number of function evaluations per iteration
  - number of gradient evaluations
  - number of gradient evaluations per iteration
"""
	num_rows = len(S) + 1
	num_cols = 6
	R = []
	R.append([])
	R[0].append("Algorithm")
	R[0].append("i")
	R[0].append("f")
	R[0].append("f/i")
	R[0].append("g")
	R[0].append("g/i")
	for ri in range(num_rows):
		R.append([])
		for ci in range(num_cols):
			R[ri+1].append("")
	column_widths = (30, 6, 6, 6, 6, 6)
	
	for si in range(len(S)):
		#results = minimize(S[si], DefaultSolverSetup(),
		                   #f, sc, x0, NoConstraints(), 0, False)
		results = S[si].solve(f, x0, sc, Solver.DefaultSetup(), NoConstraints(), False)
		R[si+1][0] = S[si].get_name()
		R[si+1][1] = results.num_iter
		R[si+1][2] = results.num_func_eval
		R[si+1][3] = 1.0*results.num_func_eval / results.num_iter
		R[si+1][4] = results.num_grad_eval
		R[si+1][5] = 1.0*results.num_grad_eval / results.num_iter
	
	print_table(R, num_rows, num_cols, column_widths)

def main():
	S = [ DSQA(),
        GSLfdfsolver('vector_bfgs'),
	      GSLfdfsolver('vector_bfgs2'),
	      GSLfdfsolver('conjugate_fr'),
	      GSLfdfsolver('conjugate_pr') ]
	
	f = ExtendedRosenbrock(n=6)
	
	solver_results_table(S, f.otk_instance, f.x0, f.stopcrit, 10000)

if __name__ == "__main__":
	main()
