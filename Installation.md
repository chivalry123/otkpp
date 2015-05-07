# OTK++ #

## Requirements ##

OTK++ has been developed and tested on Linux (Ubuntu and Debian). Users are also encouraged to test it on other platforms. It has been tested to compile with g++ 4.2 and 4.3. A recent version of uBLAS is also required, and 1.34 is the oldest version it is known to compile with. Using CMake version 2.6 is strongly recommended, but 2.4 might also work with some workarounds.

**Required dependencies:**
  * uBLAS >= 1.34
  * g++ >= 4.2
  * CMake >= 2.6

OTK++ can be linked to several libraries that enable additional functionality. Enabling and disabling compilation of OTK++ with these libraries can be done with optional CMake flags.

**Optional dependencies:**

|  Library/program  |  Version  |  CMake flag       |  Enables |
|:------------------|:----------|:------------------|:---------|
|  GSL              | >= 1.11   | -DWITH\_GSL=ON/OFF |  GSL minimization algorithms |
|  GNU libmatheval  | >= 1.1.3  | -DWITH\_LIBMATHEVAL=ON/OFF | symbolic function evaluation |
|  gfortran         | >= 4.2    | -DWITH\_FORTRAN=ON/OFF  |  algorithms implemented in Fortran (e.g. LMBM) |
|  Qt               | >= 4.3    | -DWITH\_QT=ON/OFF   |    Qt-based GUI |
|  Qwt              | >= 5.1    | -DWITH\_QT=ON/OFF   |    2d plotting widgets |
|  QwtPlot3D        | >= 0.2    | -DWITH\_QT=ON/OFF   |   3d plotting widgets |

The above flags are set to 'ON' by default.

## Installation ##

OTK++ uses CMake as its build system. To install the C++ libraries,
extract the otkpp tarball and type the following commands in the extracted directory:

```
  cmake . <optional flags>   <- put your CMake flags here
  make
  sudo make install
```

By default, headers will be installed in /usr/local/include/otkpp,
libraries in /usr/local/lib and binaries in /usr/local/bin. You can
change the install prefix by appending -DCMAKE\_INSTALL\_PREFIX=`<your-prefix>`
to the list of CMake flags.

<b>NOTE:</b> If the install prefix is not /usr, you may need to set the LD\_LIBRARY\_PATH
environment variable in order to use the libraries:

```
export LD_LIBRARY_PATH=<path>
```

where <tt>path</tt> points to the directory where the libraries are installed.


---


# PyOTK #

PyOTK is a Python extension for OTK++. It uses Boost.Python for exposing the OTK++ classes into Python. Several utilities for analyzing and visualizing results of minimization algorithms are implemented in PyOTK.

## Requirements ##

As PyOTK is an extension of OTK++, it requires the OTK++ libraries, and they must have been installed either with prefix /usr or /usr/local.

PyOTK has been tested to compile with Python 2.5/2.6 and
Boost.Python 1.34. It also requires the following Python libraries:

  * Matplotlib >=  0.98
  * NumPy >=  1.1.0
  * SciPy >=  0.6.0

**NOTE:** Python 3.0 and newer are not currently supported.

## Installation ##

PyOTK uses distutils for compiling the C++ interface libraries
and copying the Python scripts to appropriate directories in
the system path. To install PyOTK, extract the pyotk tarball
and type the following command:

```
  python setup.py install
```

**NOTE:** The functionality of the Python interface depends
on the flags the OTK++ library was compiled with. The setup.py
script automatically parses the configuration file generated
by the OTK++ install script and reads the variables it
was configured with.