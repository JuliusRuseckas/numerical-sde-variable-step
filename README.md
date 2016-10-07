# Numerical solution of stochastic differential equations using a variable time step

This is a source code for the numerical solution of nonlinear stochastic differential equations generating signals with _1/f_ noise. The equtions are described in Refs. [1-2]. The code can be adapted to solve various equations of this type.

The particular case of the equation with the known analytical expression for the power spectral density and the method of solution are described by the files in the directiory 'equations'.

The main file of the program is [spectrum.c](spectrum.c). The actual numerical solution of the equation is in the file [compute_signal.c](compute_signal.c).

The [GNU Scientific Library (GSL)](http://www.gnu.org/software/gsl/) is used for random number generation and histograms. The [FFTW library](http://www.fftw.org/) version 3.x is used for discrete Fourier transforms.

Before calculations the program 'spectrum' reads a FFTW plan from a file in the current directory with the name 'wisdom'. The wisdom file can be generated with the program 'makewisdom'

The program 'spectrum' outputs the sample signal into the file 'signal.dat', the steady-state probability distribution of the signal into the file 'distribution.dat' and the power spectral density into the file 'spectrum.dat'

[Gnuplot](http://www.gnuplot.info/) scripts for plotting the stationary probability distribution, the power spectral density and for comparison with the analytical expression are provided in the directory [test](test)

[1] B. Kaulakys and J. Ruseckas, _Stochastic nonlinear differential equation generating 1/f noise_, Phys. Rev. E **70**, 020101 (2004). doi: [10.1103/PhysRevE.70.020101](https://doi.org/10.1103/PhysRevE.70.020101)
    
[2] B. Kaulakys, J. Ruseckas, V. Gontis and M. Alaburda, _Nonlinear stochastic models of 1/f noise and power-law distributions_, Physica A **365**, 217-221 (2006). doi: [10.1016/j.physa.2006.01.017](https://doi.org/10.1016/j.physa.2006.01.017)
