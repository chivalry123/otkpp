
from native import *
from numpy import logspace

def scaling_analysis(s):
	min_alpha_exp = -6
	max_alpha_exp = 0
	num_alpha_val = 25
	alpha_val = logspace(min_alpha_exp, max_alpha_exp, num_alpha_val)
	max_num_iter = 1000
	
	for alpha in alpha_val:
		f_expr = '(' + str(alpha) + '*x^2)+y^2'
		f = Function(f_expr)
		x0 = (3.0/alpha, 2.0)
		
		results = minimize(s, DefaultSolverSetup(),
                       f, GradNormTest(1e-8),
                       x0, NoConstraints(), 0, False)
		
		# TODO...
		#print results.num_iter

def main():
	s = LinminBFGS(BFGSLmType.morethuente)
	scaling_analysis(s)

if __name__ == "__main__":
	main()
