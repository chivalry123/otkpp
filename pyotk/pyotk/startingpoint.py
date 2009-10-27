
from numpy import *
from plot2d import plot_contours
from pylab import figure, imshow, show, subplot
from testproblems import *

def starting_point_analysis(s, f, p_min, x1, x2, y1, y2, n, tol=1e-3):
	if f.get_n() != 2:
		raise ValueError('only functions of two variables are supported')
	
	if len(p_min) > 6:
		raise ValueError('maximum number of 6 minima allowed')
	
	C = [ (1, 0, 0), (0, 1, 0), (0, 0, 1), (0, 1, 1), (1, 0, 1), (1, 1, 0) ]
	I = zeros((n, n, 3))
	
	dx = x2 - x1
	dy = y2 - y1
	
	for i in range(n):
		y = y1 + 1.0*i/(n-1) * dy
		for j in range(n):
			x = x1 + 1.0*j/(n-1) * dx
			
			results = minimize(s, DefaultSolverSetup(),
                         f, GradNormTest(eps=1e-6),
                         (x, y), NoConstraints(), 0, False)
			p_final = array(results.x_final)
			
			found_p_min = None
			ci = 1
			for p in p_min:
				dp = p - p_final
				if sqrt(dot(dp, dp)) < tol:
					found_p_min = p
					break
				ci = ci + 1
			
			if found_p_min is not None:
				I[i][j][:] = C[ci][:]
			else:
				I[i][j][:] = 1
	
	figure()
	imshow(I, extent=(x1, x2, y1, y2))
	plot_contours(f, x1, x2, y1, y2, 0.1, 500, True, False)
	show()

def main():
	f = Function("(x^2+y-11)^2+(x+y^2-7)^2")
	p_min = [ (3, 2),
            (-2.805118, 3.131312),
            (-3.779310, -3.283186),
            (3.584428, -1.848126) ]
	
	s = LinminBFGS(BFGSLmType.morethuente)
	starting_point_analysis(s, f, p_min, -5, 5, -5, 5, 100)
	s = SteihaugSR1()
	starting_point_analysis(s, f, p_min, -5, 5, -5, 5, 100)

if __name__ == "__main__":
	main()
