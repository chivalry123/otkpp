"""Implements tools for computing performance profiles.

This module implements an utility for computing 
the Dolan and More performance profiles for a 
set of solvers given set on a given set of test 
problems. For the definition of a performance 
profile, see

E.D. Dolan and J.J. More, Benchmarking optimization 
software with performance profiles, Mathematical 
Programming 91 (2002), no. 2, 201-213
"""

from native import *
from matplotlib import rc
from matplotlib.pyplot import *
from numpy import *
from testproblems import *

def performance_profile(S, P, plot_results):
	"""For a given set of solver S and a given set of 
test problems P, compute the Dolan and More performance 
profiles. A triplet (R,tau,rho) is returned. R is 
a mxn matrix containing the computed performance 
ratios, where m and n denotes the sizes of P and S, 
respectively. The return values tau and rho values 
represent the x and y values used for plotting the 
data. If the boolean value plot_results is set to 
True, the results are also plotted."""
	R_MAX = 1e99
	
	T = zeros((len(P), len(S)))
	R = zeros((len(P), len(S)))
	
	si = 0
	for s in S:
		print 'Algorithm:', s.get_name()
		print "%-25s%-13s%-15s%-15s%-8s" % ('Test function', 
					'Time', 'Term.val.', '#i   #f   #g', 'Time/iter.')
		
		pi = 0
		for p in P:
			results = s.solve(p.f, p.x0, p.stopcrit, 
                        Solver.DefaultSetup(), NoConstraints(), True)
			
			if results.converged == True:
				print "%-25s%-13.2f%-15.4e%-5d%-5d%-5d%-8.2f" % (p.name, 
          results.time, results.term_val, results.num_iter, 
          results.num_func_eval, results.num_grad_eval, 
          1.0 * results.time / results.num_iter)
				T[pi, si] = results.time
			else:
				print "%-25s%-13s%-15.4e%-5s%-5s%-5s%-8s" % (p.name,
              'Failure', results.term_val, '-', '-', '-', '-')
				T[pi, si] = nan
			
			pi = pi + 1
		si = si + 1

	d = zeros(len(P))
	for pi in range(len(P)):
		d[pi] = nanmin(T[pi, :])
	for si in range(len(S)):
		for pi in range(len(P)):
			if not isnan(T[pi, si]) and not isnan(d[pi]):
				R[pi, si] = T[pi, si] / d[pi]
			else:
				R[pi, si] = R_MAX
	
	tau = linspace(0, 1.1 * log2(R[R < R_MAX].max()), 20)
	rho = zeros((len(tau), len(S)))
	
	for si in range(len(S)):
		for ti in range(len(tau)):
			rho[ti, si] = sum(log2(R[:, si]) <= tau[ti])
	rho = rho / len(P)
	
	if plot_results == True:
		markers = ['^', 's', 'x', 'o', '+']
		rc('text', usetex=True)
		figure()
		plots = []
		for si in range(len(S)):
			plots += plot(tau, rho[:, si], '-' + markers[si], 
				linewidth=2, markersize=6)
		legends = []
		for s in S:
			legends.append(s.get_name().replace('_', '\_'))
		legend(plots, legends, loc='lower right', shadow=True)
		xlabel(r'\tau')
		ylabel(r'\rho_s(2^\tau)')
		xlim(0, tau[len(tau) - 1])
		ylim(0, 1.05)
		grid(True)
		show()
	
	return (R, tau, rho)

def main():
	S = [ LRWWSimplex(),
        DSQA(),
        #DFQAS(),
        SteihaugSR1(),
        #ConjGradMT(ConjGradType.FR),
        #ConjGradMT(ConjGradType.PR),
        LinminBFGS(LinminBFGS.LinminType.morethuente) ]
	
	P1 = [ PowellBadlyScaled(),
         BrownBadlyScaled(),
         Beale(),
         HelicalValley(),
         Gaussian(),
         Gulf(m=5),
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
	
	P2 = [ #PowellBadlyScaled(gEvalType=DerivEvalType.fdiff_central_2),
         #BrownBadlyScaled(gEvalType=DerivEvalType.fdiff_central_2),
         #Beale(gEvalType=DerivEvalType.fdiff_central_2),
         #HelicalValley(gEvalType=DerivEvalType.fdiff_central_2),
         #Gaussian(gEvalType=DerivEvalType.fdiff_central_2),
         Gulf(m=5, gEvalType=DerivEvalType.fdiff_central_2),
         Box(m=5, gEvalType=DerivEvalType.fdiff_central_2),
         #Wood(gEvalType=DerivEvalType.fdiff_central_2),
         BrownDennis(m=20, gEvalType=DerivEvalType.fdiff_central_2),
         BiggsEXP6(m=13, gEvalType=DerivEvalType.fdiff_central_2),
         Watson(n=6, gEvalType=DerivEvalType.fdiff_central_2),
         ExtendedRosenbrock(n=32, gEvalType=DerivEvalType.fdiff_central_2),
         ExtendedPowellSingular(n=16, gEvalType=DerivEvalType.fdiff_central_2),
         PenaltyFunctionI(n=10, gEvalType=DerivEvalType.fdiff_central_2),
         PenaltyFunctionII(n=10, gEvalType=DerivEvalType.fdiff_central_2),
         VariablyDimensioned(n=20, gEvalType=DerivEvalType.fdiff_central_2),
         Trigonometric(n=7, gEvalType=DerivEvalType.fdiff_central_2),
         ChebyQuad(n=8, m=8, gEvalType=DerivEvalType.fdiff_central_2) ]
	
	performance_profile(S, P1, True)

if __name__ == "__main__":
	main()
