"""Implements tools for analyzing convergence rates.

This module implements methods for comparing convergence 
rates of algorithms and plotting given quantities as a 
function of used iterations.
"""

from native import *
from matplotlib import rc
from matplotlib.pyplot import *
from numpy import *
from scipy.linalg import norm
from testproblems import *

def compare_convergence(S, P, eps=1e-12, max_num_iter=50000):
	"""Compare convergence of the algorithms given in the set 
S on test problems given in the set P. For each test problem,
the distance to the known minimizer is plotted as a function 
of number of used iterations. The convergence graphs of each 
algorithm are plotted in the same figure. Note that only test 
problems with a known minimizer can be used with this function. 
Otherwise, a ValueError is raised."""
	I = range(max_num_iter)
	rc('text', usetex=True)
	
	for p in P:
		if not isinstance(p.stopcrit, XDistToMinTest):
			raise ValueError("Test problem " + p.name + " does not have a known minimizer.")
		
		stopcrit = XDistToMinTest(p.x_min, eps=eps, relative=False)
		
		R = zeros((max_num_iter, len(S)))
		figure()
	
		max_norm = 0
		max_used_iter = 0
		si = 0
		for s in S:
			results = minimize(s, DefaultSolverSetup(),
			                   p.otk_instance, stopcrit,
			                   p.x0, NoConstraints(), 0, False)
			
			for i in range(0, results.num_iter):
				R[i, si] = norm(array(results.states[i].x) - p.x_min)
				if R[i, si] == 0:
					R[i, si] = eps
				max_norm = max(max_norm, R[i, si])
			max_used_iter = max(results.num_iter, max_used_iter)
			si = si + 1
		
		plots = semilogy(I, R, linewidth=2)
		title(p.name)
		xlim(0, 1.2 * max_used_iter)
		ylim(eps, 10 * max_norm)
		xlabel('Iterations')
		ylabel(r'\|\mathbf{x}_k-\mathbf{x}^*\|')
		legends = []
		for i in range(len(S)):
			legends.append(S[i].get_name().replace('_', '\_'))
		legend(plots, legends, loc='upper right', shadow=True)
		grid(True)
		
		show()
	
	return

def convergence_plot(results, quantities):
	"""From a results class produced by pyotk_driver.minimize(), 
plot the requested quantities as a function of number of used 
iterations. The list of quantities may contain the following 
name strings:
  disttominimizer    distance to the a priori known minimizer 
                     of the test problem
  funcval            objective function value
  gradnorm           gradient norm
  simplexvolume      simplex volume
"""
	X = range(1, results.num_iter + 1)
	Y = zeros((results.num_iter, len(quantities)))
	
	qi = 0
	for q in quantities:
		for si in range(results.num_iter):
			s = results.states[si]
			if q == 'disttominimizer':
				Y[si, qi] = norm(s.x - results.x_min)
			elif q == 'funcval':
				Y[si, qi] = s.f
			elif q == 'gradnorm':
				Y[si, qi] = norm(s.g)
			elif q == 'simplexvolume':
				Y[si, qi] = s.V
		qi = qi + 1
	
	rc('text', usetex=True)
	figure()
	plots = semilogy(X, Y, linewidth=2)
	xlim(1, results.num_iter)
	
	legends = []
	for q in quantities:
		if q == 'disttominimizer':
			legends.append(r'|$\mathbf{x}_k-\mathbf{x}^*$|')
		elif q == 'funcval':
			legends.append(r'$f(\mathbf{x}_k)$')
		elif q == 'gradnorm':
			legends.append(r'$\|\nabla f(\mathbf{x}_k)\|$')
		elif q == 'simplexvolume':
			legends.append(r'$|V(S_k)|$')
	
	legend(plots, legends, loc='lower left', shadow=True)
	xlabel('Iterations')
	grid(True)
	show()

def main():
	S = [ LinminBFGS(BFGSLmType.morethuente),
	      LinminBFGS(BFGSLmType.fletcher),
	      ConjGradMT(ConjGradType.FR),
	      ConjGradMT(ConjGradType.PR) ]
	
	P = [ Beale(),
	      HelicalValley(),
	      Wood(),
	      ExtendedRosenbrock(n=10) ]
	
	compare_convergence(S, P);
	
	tf = ExtendedRosenbrock(8)
	results = minimize(GSLfdfsolver('vector_bfgs2'),
                     DefaultSolverSetup(), tf.otk_instance,
                     GradNormTest(eps=1e-8), tf.x0,
                     NoConstraints(), 0, False)
	quantities = ['gradnorm', 'funcval']
	
	convergence_plot(results, quantities)

if __name__ == "__main__":
	main()
