from distutils.core import setup, Extension
from string import *
import sys

def read_config_h():
	with_fortran = False
	with_gsl = False
	
	config_file = '/usr/include/otkpp/config.h'
	try:
		f = open(config_file, 'r')
	except IOError:
		config_file = '/usr/local/include/otkpp/config.h'
		try:
			f = open(config_file, 'r')
		except IOError:
			print 'No config.h found from /usr/local/include/otkpp or /usr/include/otkpp. Aborting.'
			sys.exit()
		
	for l in f:
		if find(l, '#define WITH_FORTRAN') != -1:
			with_fortran = True
		if find(l, '#define WITH_GSL') != -1:
			with_gsl = True
	
	f.close()
	
	return [with_fortran, with_gsl]

[with_fortran, with_gsl] = read_config_h()

macros = []
if with_fortran == True:
	macros.append(('WITH_FORTRAN', 1))
if with_gsl == True:
	macros.append(('WITH_GSL', 1))

libs = ['otkpp', 'boost_python', 'matheval', 'rt']
if with_fortran == True:
	libs.append('otkpp_fortran')
	libs.append('gfortran')
if with_gsl == True:
	libs.append('gsl')
	libs.append('gslcblas')

pyotk_native = Extension(
	name = 'pyotk.native',
	sources = ['native.cpp'],
	include_dirs = ['/usr/include', '/usr/local/include'],
	library_dirs = ['/usr/lib', '/usr/local/lib'],
	libraries = libs,
	define_macros = macros)

libs = ['otkpp', 'boost_python']
if with_fortran == True:
	libs.append('gfortran')
pyotk_plotutils = Extension(
	name = 'pyotk.plotutils',
	sources = ['plotutils.cpp'],
	include_dirs = ['/usr/include', '/usr/local/include'],
	library_dirs = ['/usr/lib', '/usr/local/lib'],
	libraries = libs)

setup(
	name = 'pyotk',
	version = '0.1.1',
	packages = ['pyotk'],
	ext_modules = [pyotk_native, pyotk_plotutils],
	#install_requires = ['numpy>=1.1.0', 'scipy>=0.6.0', 'matplotlib>=0.98.0'],
	author = 'Seppo Pulkkinen',
	author_email = 'seppul@utu.fi',
	description = 'Python interface for OTK++',
	keywords = 'otk++ nonlinear optimization c++ python',
	url = 'http://otkpp.sourceforge.net'
)
