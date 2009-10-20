
from inspect import getargspec
import native
from native import *

class TestFunction:
	def __init__(self, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic, n=0, m=0):
		if fEvalType == FuncEvalType.symbolic:
			self.otk_instance = Function(self.generate_expression(), gEvalType)
		else:
			argspec = getargspec(self.__init__)[0]
			if not 'n' in argspec and not 'm' in argspec:
				eval_str = 'libpyotk_driver.' + self.__class__.__name__ + '(gEvalType)'
			elif 'm' not in argspec:
				eval_str = 'libpyotk_driver.' + self.__class__.__name__ + '(' + str(n) + ',gEvalType)'
			elif 'n' not in argspec:
				eval_str = 'libpyotk_driver.' + self.__class__.__name__ + '(' + str(m) + ',gEvalType)'
			else:
				eval_str = 'libpyotk_driver.' + self.__class__.__name__ + '(' + str(n) + ',' + str(m) + ',gEvalType)'
			
			self.otk_instance = eval(eval_str)

class PlotSpec:
	def __init__(self, x_range, y_range, z_range, z_logscale):
		self.x_range = x_range
		self.y_range = y_range
		self.z_range = z_range
		self.z_logscale = z_logscale

class PowellBadlyScaled(TestFunction):
	name = 'Powell badly scaled'
	x0 = (0, 1)
	stopcrit = FDistToMinTest(f_min=0, eps=1e-14, relative=False);
	plot_spec = PlotSpec((-5e-5, 2e-4), (-1, 10), (1e-6, 1000), True)
	
	def __init__(self, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		TestFunction.__init__(self, fEvalType, gEvalType)
	
	def generate_expression(self):
		return "(1e4*x*y-1)^2+(exp(-x)+exp(-y)-1.0001)^2"
	
class BrownBadlyScaled(TestFunction):
	name = 'Brown badly scaled'
	x0 = (1, 1)
	x_min = (1e6, 2e-6)
	stopcrit = XDistToMinTest(x_min=(1e6, 2e-6), eps=1e-6, relative=False)
	plot_spec = PlotSpec((1e6-1e-4, 1e6+1e-4),
	                     (2e-6-1e-10, 2e-6+1e-10),
	                     (1e-16, 1e-6), True)
	
	def __init__(self, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		TestFunction.__init__(self, fEvalType, gEvalType)
	
	def generate_expression(self):
		return '(x-1e6)^2+(y-2e-6)^2+(x*y-2)^2'

class Beale(TestFunction):
	name = 'Beale'
	x0 = (1, 1)
	x_min = (3, 0.5)
	stopcrit = XDistToMinTest(x_min=(3, 0.5), eps=1e-6, relative=False)
	plot_spec = PlotSpec((0.25, 4.25), (-1, 1.5), (1e-3, 500), True)
	
	def __init__(self, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		TestFunction.__init__(self, fEvalType, gEvalType)
	
	def generate_expression(self):
		return '(1.5-x*(1-y))^2+(2.25-x*(1-y^2))^2+(2.625-x*(1-y^3))^2'

class HelicalValley(TestFunction):
	name = 'Helical valley'
	x0 = (-1, 0, 0)
	x_min = (1, 0, 0)
	stopcrit = XDistToMinTest(x_min=(1, 0, 0), eps=1e-6, relative=False)
	plot_spec = None
	
	def __init__(self, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		TestFunction.__init__(self, fEvalType, gEvalType)
	
	def generate_expression(self):
		e = ''
		
		atan_term = '1/(2*pi)*atan(x2/x1)'
		e += '100.0*(step(x1)*' + atan_term + '+step(-x1)*(' + atan_term + '+0.5))^2+'
		e += '100.0*(sqrt(x1^2+x2^2) - 1.0)^2+'
		e += 'x3^2'
		
		return e

class Gaussian(TestFunction):
	name = 'Gaussian'
	x0 = (0.4, 1, 0)
	stopcrit = FDistToMinTest(f_min=1.12793e-8, eps=1e-4, relative=True)
	plot_spec = None
	
	def __init__(self, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		TestFunction.__init__(self, fEvalType, gEvalType)
	
	def generate_expression(self):
		y = (0.0009, 0.0044, 0.0175, 0.0540, 0.1295, 0.2420, 0.3521, 0.3989, 
				0.3521, 0.2420, 0.1295, 0.0540, 0.0175, 0.0044, 0.0009)
		
		e = ''
		for i in range(1, 16):
			ti = '(8-' + str(i) + ')/2'
			yi = str(y[i-1])
			e += '(x1*exp((-x2*(' + ti + '-x3)^2)/2)-' + yi + ')^2'
			
			if i < 15:
				e += '+'
		
		return e
			

class Gulf(TestFunction):
	name = 'Gulf'
	x0 = (5, 2.5, 0.15)
	x_min = (50, 25, 1.5)
	stopcrit = XDistToMinTest(x_min=(50, 25, 1.5), eps=1e-6)
	plot_spec = None

	def __init__(self, m, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		if m < 3 or m > 100:
			raise ValueError('must be 3<=m<=100')
		self.m = m
		TestFunction.__init__(self, fEvalType, gEvalType, m=m)
		
	def generate_expression(self):
		e = ''
		for i in range(1, self.m + 1):
			ti = str(i) + '/100'
			yi = '(25+(-50*log(' + ti + '))^(2/3))'
			mi = str(self.m*i)
			e += '(exp(-abs(' + yi + '-x2)^x3/x1)-' + ti + ')^2'
			
			if i < self.m:
				e += '+'
		
		return e

class Box(TestFunction):
	name = 'Box'
	x0 = (0, 10, 20)
	stopcrit = FDistToMinTest(f_min=0, eps=1e-6, relative=False)
	plot_spec = None

	def __init__(self, m, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		if m < 3:
			raise ValueError('must be m>=3')
		self.m = m
		TestFunction.__init__(self, fEvalType, gEvalType, m=m)
		
	def generate_expression(self):
		e = ''
		for i in range(1, self.m + 1):
			ti = '0.1*' + str(i)
			e += '(exp(-' + ti + '*x1)-exp(-' + ti + '*x2)-x3*(exp(-' + ti + ')-exp(-10*' + ti + ')))^2'
      
			if i < self.m:
				e += "+";
		
		return e

class Wood(TestFunction):
	name = 'Wood'
	x0 = (-3, -1, -3, -1)
	x_min = (1, 1, 1, 1)
	stopcrit = XDistToMinTest(x_min=(1, 1, 1, 1), eps=1e-6, relative=False)
	plot_spec = None
	
	def __init__(self, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		TestFunction.__init__(self, fEvalType, gEvalType)
		
	def generate_expression(self):
		e = ''
		e += '100*(x2-x1^2)^2+'
		e += '(1-x1)^2+'
		e += '90*(x4-x3^2)^2+'
		e += '(1-x3)^2+'
		e += '10*(x2+x4-2)^2+'
		e += '1/10*(x2-x4)^2'
		
		return e
  
class BrownDennis(TestFunction):
	name = 'Brown and Dennis'
	x0 = (25, 5, -5, -1)
	stopcrit = FDistToMinTest(f_min=85822.2, eps=0.1, relative=False)
	plot_spec = None
	
	def __init__(self, m, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		if m < 4:
			raise ValueError('must be m>=4')
		self.m = m
		TestFunction.__init__(self, fEvalType, gEvalType, m=m)
		
	def generate_expression(self):
		e = ''
		for i in range(1, self.m + 1):
			ti = str(i) + '/5'
			e += '((x1+' + ti + '*x2-exp(' + ti + '))^2+(x3+x4*sin(' + ti + ')-cos(' + ti + '))^2)^2'
			if	i < self.m:
				e += "+"
		
		return e

class BiggsEXP6(TestFunction):
	name = 'Biggs EXP6'
	x0 = (1, 2, 1, 1, 1, 1)
	x_min = (1, 10, 1, 5, 4, 3)
	stopcrit = FDistToMinTest(f_min=5.65565e-3, eps=1e-4, relative=True)
	plot_spec = None
	
	def __init__(self, m, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		if m < 6:
			raise ValueError('must be m>=6')
		self.m = m
		TestFunction.__init__(self, fEvalType, gEvalType, m=m)
		
	def generate_expression(self):
		e = ''
		for i in range(1, self.m + 1):
			ti = '0.1*' + str(i)
			yi = '(exp(-' + ti + ')-5.0*exp(-10.0*' + ti + ')+3*exp(-4.0*' + ti + '))'
			e += '(x3*exp(-' + ti + '*x1)-x4*exp(-' + ti + '*x2)+x6*exp(-' + ti + '*x5)-' + yi + ')^2'
			if i < self.m:
				e += '+'
			
		return e

class Watson(TestFunction):
	name = 'Watson'
	stopcrit = FDistToMinTest(f_min=2.28767e-3, eps=1e-4, relative=True)
	plot_spec = PlotSpec((-1.5, 0.4), (0, 2), (0.25, 500), True)
	
	def __init__(self, n, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		if n < 2 or n > 31:
			raise ValueError('must be 2<=n<=31')
		self.n = n
		TestFunction.__init__(self, fEvalType, gEvalType, n=n)
		
		x0 = []
		for i in range(n):
			x0.append(0)
		self.x0 = tuple(x0)
		
	def generate_expression(self):
		e = ''
		for i in range(1, 30):
			sum1 = ''
			sum2 = ''
			ti = str(i) + '/29'
			
			for j in range(2, self.n + 1):
				sum1 += '(' + str(j) + '-1)*x' + str(j) + '*(' + ti + ')^(' + str(j) + '-2)'
				if j < self.n:
					sum1 += '+'
			
			for j in range(1, self.n + 1):
				sum2 += 'x' + str(j) + '*(' + ti + ')^(' + str(j) + '-1)'
				if j < self.n:
					sum2 += '+'
			
			e += '(' + sum1 + '-(' + sum2 + ')^2-1)^2'
			e += '+'
		
		e += 'x1^2+(x2-x1^2-1)^2'
		
		return e

class ExtendedRosenbrock(TestFunction):
	name = 'Extended Rosenbrock'
	plot_spec = PlotSpec((-1.5, 1.4), (-0.25, 1.25), (1e-3, 1000), True)

	def __init__(self, n, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		if n % 2 != 0:
			raise ValueError("n must be even")
		
		self.n = n
		TestFunction.__init__(self, fEvalType, gEvalType, n=n)
		
		x0 = []
		for i in range(n):
			if i % 2 == 0:
				x0.append(-1.2)
			else:
				x0.append(1)
		self.x0 = tuple(x0)

		x_min = []
		for i in range(n):
			x_min.append(1)
		self.x_min = tuple(x_min)
		self.stopcrit = XDistToMinTest(x_min=self.x_min, eps=1e-6, relative=False)
	
	def generate_expression(self):
		e = ''
		for i in range(1, self.n, 2):
			x1 = 'x' + str(i)
			x2 = 'x' + str(i + 1)
			
			term1 = '100*(' + x1 + '*' + x1 + '-' + x2 + ')*(' + x1 + '*' + x1 + '-' + x2 + ')'
			term2 = '(1-' + x1 + ')*(1-' + x1 + ')'
			e += term1 + '+' + term2
			
			if i < self.n - 1:
				e += '+'
		
		return e

class ExtendedPowellSingular(TestFunction):
	name = 'Extended Powell singular'
	plot_spec = None
	
	def __init__(self, n, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		if n % 4 != 0:
			raise ValueError("n must be a multiple of 4")
		self.n = n
		
		TestFunction.__init__(self, fEvalType, gEvalType, n=n)
		
		x0 = []
		for i in range(n):
			if i % 4 == 0:
				x0.append(3)
			elif i % 4 == 1:
				x0.append(-1)
			elif i % 4 == 2:
				x0.append(0)
			elif i % 4 == 3:
				x0.append(1)
		self.x0 = tuple(x0)

		x_min = []
		for i in range(n):
			x_min.append(0)
		self.x_min = tuple(x_min)
		self.stopcrit = XDistToMinTest(x_min=self.x_min, eps=1e-6, relative=False)
		
	def generate_expression(self):
		e = ''
		for i in range(0, self.n, 4):
			fix = 'x' + str(i) + '+10.0*x' + str(i+1);
			e += '(' + fix + ')^2+'
      
			fix = 'x' + str(i+2) + '-x' + str(i+3)
			e += '5.0*(' + fix + ')^2+'
      
			fix = '(x' + str(i+1) + '+2.0*x' + str(i+2) + ')^2'
			e += '(' + fix + ')^2+'
      
			fix = '(x' + str(i) + '-x' + str(i+3) + ')^2'
			e += '10.0*(' + fix + ')^2'
			
			if i < self.n - 4:
				e += '+'
		
		return e
		

class PenaltyFunctionI(TestFunction):
	name = 'Penalty function I'
	stopcrit = FDistToMinTest(f_min=7.08765e-5, eps=1e-4, relative=True)
	plot_spec = PlotSpec((-1, 1), (-1, 1), (1e-6, 10), True)

	def __init__(self, n, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		self.n = n
		
		TestFunction.__init__(self, fEvalType, gEvalType, n=n)
		
		x0 = []
		for i in range(n):
			x0.append(i + 1)
		self.x0 = tuple(x0)
	
	def generate_expression(self):
		e = ''
		for i in range(1, self.n + 1):
			fix = 'x' + str(i) + '-1.0'
			e += '10^-5*(' + fix + ')^2+'
    
		fix = "";
		for i in range(1, self.n + 1):
			fix += 'x' + str(i) + '^2+'
		fix += '-0.25'
    
		e += '(' + fix + ')^2'
    
		return e;

class PenaltyFunctionII(TestFunction):
	name = 'Penalty function II'
	stopcrit = FDistToMinTest(f_min=2.93660e-4, eps=1e-4, relative=True)
	plot_spec = PlotSpec((-1, 1.25), (-2, 2), (1e-4, 50), True)

	def __init__(self, n, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		self.n = n
		
		TestFunction.__init__(self, fEvalType, gEvalType, n=n)
		
		x0 = []
		for i in range(n):
			x0.append(0.5)
		self.x0 = tuple(x0)
		
	def generate_expression(self):
		e = '(x1-0.2)^2+'
    
		for i in range(2, self.n + 1):
			fix = ''
      
			yi = 'exp(' + str(i) + '/10)+' + 'exp(' + str(i - 1) + '/10)'
      
			fix += 'exp(x' + str(i) + '/10)+'
			fix += 'exp(x' + str(i - 1) + '/10)-'
			fix += '(' + yi + ')'
      
			e += '10^-5*(' + fix + ')^2+'
    
		for i in range(self.n + 1, 2 * self.n):
			fix = 'exp((x' + str(i - self.n + 1) + ')/10)-exp(-1/10)'
			e += '10^-5*(' + fix + ')^2+'
    
		sum_str = ''
		for i in range(1, self.n + 1):
			sum_str += str(self.n-i+1) + '*x' + str(i) + '^2+'
		sum_str += '-1'
    
		e += '(' + sum_str + ')^2'
    
		return e
		
class VariablyDimensioned(TestFunction):
	name = 'Variably dimensioned'
	plot_spec = PlotSpec((0, 2), (0, 2), (1e-4, 100), True)
	
	def __init__(self, n, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		self.n = n
		
		TestFunction.__init__(self, fEvalType, gEvalType, n=n)
		
		x0 = []
		for i in range(n):
			x0.append(1.0 - (i+1.0)/n)
		self.x0 = tuple(x0)
		
		x_min = []
		for i in range(n):
			x_min.append(1)
		self.x_min = tuple(x_min)
		self.stopcrit = XDistToMinTest(x_min=self.x_min, eps=1e-6, relative=False)
		
	def generate_expression(self):
		e = ''
		for i in range(1, self.n + 1):
			e += '(x' + str(i) + '-1)^2+'
    
		s = ''
		for i in range(1, self.n + 1):
			s += str(i) + '*(x' + str(i) + '-1)'
			if i < self.n:
				s += '+'
		e += '(' + s + ')^2+(' + s + ')^4'
		
		return e

class Trigonometric(TestFunction):
	name = 'Trigonometric'
	stopcrit = FDistToMinTest(f_min=0, eps=1e-5, relative=False)
	plot_spec = PlotSpec((-10, 10), (-10, 10), (1e-5, 140), False)
	
	def __init__(self, n, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		self.n = n
		
		TestFunction.__init__(self, fEvalType, gEvalType, n=n)
		
		x0 = []
		for i in range(n):
			x0.append(1.0/n)
		self.x0 = tuple(x0)
	
	def generate_expression(self):
		e = ''
		for i in range(1, self.n + 1):
			fix = ''
			fix += str(self.n) + '-('
      
			for j in range(1, self.n + 1):
				fix += 'cos(x' + str(j) + ')'
				if j < self.n:
					fix += '+'
      
			fix += ')+' + str(i) + '*(1-cos(x' + str(i) + '))-sin(x' + str(i) + ')'
      
			e += '(' + fix + ')^2'
			if i < self.n:
				e += '+'
    
		return e

class ChebyQuad(TestFunction):
	name = 'Chebyquad'
	stopcrit = FDistToMinTest(f_min=3.51687e-3, eps=1e-5, relative=True)
	plot_spec = PlotSpec((0, 1), (0, 1), (1, 10), False)

	def __init__(self, n, m, fEvalType=FuncEvalType.symbolic, gEvalType=DerivEvalType.symbolic):
		if m < n:
			raise ValueError('must be m>=n')
		
		self.n = n
		self.m = m
		
		TestFunction.__init__(self, fEvalType, gEvalType, n=n, m=m)
		
		x0 = []
		for i in range(n):
			x0.append((i+1.0) / (n+1.0))
		self.x0 = tuple(x0)
	
	def generate_expression(self):
		e = ''
		for i in range(1, self.m + 1):
			t = ''
      
			for j in range(1, self.n + 1):
				Tim1 = '1.0'
				x_str = "x" + str(j)
				Ti = x_str
				for k in range(2, i + 1):
					prev_Ti = Ti;
					Ti = '2.0*(' + x_str + '*(' + Ti + '))-(' + Tim1 + ')'
					Tim1 = prev_Ti;
        
				t += Ti
				if j < self.n:
					t += '+'
      
			t = '(' + t + ')/' + str(self.n)
      
			if i % 2 == 0:
				t += '+1.0/(' + str(i) + '^2-1.0)'
      
			e += '(' + t + ')^2'
			if i < self.m:
				e += "+"
    
		return e
