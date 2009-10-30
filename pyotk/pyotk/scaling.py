
from native import *
from numpy import logspace,zeros
from matplotlib import rc
from matplotlib.pyplot import *

def scaling_analysis(s):
	min_alpha_exp = -6
	max_alpha_exp = 0
	num_alpha_val = 25
	max_num_iter = 1000
	
	alpha_val = logspace(min_alpha_exp, max_alpha_exp, num_alpha_val)
	num_iter = zeros(len(alpha_val))
	
	i = 0
	max_num_iter = 0
	for alpha in alpha_val:
		f_expr = '(' + str(alpha) + '*x)^2+y^2'
		f = Function(f_expr)
		x0 = (3.0/alpha, 2.0)
		
		results = minimize(s, DefaultSolverSetup(),
                       f, XDistToMinTest((0, 0), 1/alpha*1e-8),
                       x0, NoConstraints(), 0, False)
		num_iter[i] = results.num_iter
		max_num_iter = max(max_num_iter, results.num_iter)
		i = i + 1
	
	rc('text', usetex=True)
	figure()
	semilogx(alpha_val, num_iter)
	ylim(0, 1.1*max_num_iter)
	title(r'Function: $(\alpha x)^2+y^2$, algorithm: ' + s.get_name())
	xlabel(r'$\alpha$')
	ylabel('number of iterations')
	show()

def main():
	s = LinminBFGS(BFGSLmType.morethuente)
	scaling_analysis(s)
	s = ConjGradMT(ConjGradType.PR)
	scaling_analysis(s)

if __name__ == "__main__":
	main()
