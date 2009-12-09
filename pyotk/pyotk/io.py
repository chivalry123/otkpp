"""Implements methods for printing the results of minimization algorithms.
"""

from native import *

def print_results(results, verbosity):
	f = results.setup.objfunc
	
	# print each iterate and its corresponding function value
	if verbosity >= 2:
		print "%-6s" % 'k',
		
		if f.has_symbolic_expression() == True:
			var_names = f.get_variable_names()
		else:
			var_names = []
			for i in range(results.setup.n):
				var_names.append('x'+str(i+1))
		
		for i in range(results.setup.n):
			print "%-10s" % var_names[i],
		fx_str = 'f('
		for i in range(results.setup.n):
			fx_str = fx_str + var_names[i]
			if i < results.setup.n-1:
				fx_str = fx_str + ','
		fx_str = fx_str + ')'
		print fx_str
		
		for k in range(results.num_iter):
			print "%-6d" % k,
			for i in range(results.setup.n):
				print "%-10.3f" % results.states[k].x[i],
			print "%-10.5g" % results.states[k].fx
		print
	
	# print the final results
	print "%-25s" % 'Minimizer:',
	print ("(" + "%.3g,"*(results.setup.n-1) + "%.3g)") % results.x_min
	print "%-25s" % 'Minimum value:',
	print "%-10.5g" % results.f_min
	print "%-25s" % 'Iterations:',
	print "%-5d" % results.num_iter
	print "%-25s" % 'Function evaluations:',
	print "%-5d" % results.num_func_eval
