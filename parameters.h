#ifndef PARAMETERS_H
#define PARAMETERS_H

//parameters of the equation
#define SIGMA 1.0
#define KAPPA 0.01
#define XMIN  1.0

#define XINIT 1.5 //initial value
#define XOVER 1e8

//parameters of Fourier transformation
#define DELTA  0.001    //interval of time discretization
#define COUNT  1000     //number of repetitions

#define SMOOTH_NEIGHBORS 5

//parameters of histogram
#define BIN_N    10000u //number of bins in histogram
#define HIST_MIN 1e-6
#define HIST_MAX 1e7

#define LENGTH 1048576u //length of arrays

#define N_POINTS 10000  //number of points in the output

#endif
