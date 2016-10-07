#include "compute_signal.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_histogram.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "parameters.h"

void compute_signal(double * signal, const size_t length, const double delta,
    gsl_rng * r, gsl_histogram * hist){

  static double x0 = XINIT;

  puts("Building signal ...");

  double x = x0;
  double t_begin = 0.0; //Distance from the beginning of time discretization
                        //interval to current point in time

  for(size_t j=0; j<length; j++){
    //Perfom integration using method of rectangles.
    double sum = 0.0;         //Sum of signal areas
    double delta_t = t_begin; //part of time step in the new interval

    while(t_begin < delta) {
      sum += x*delta_t;

      double z = gsl_ran_gaussian_ziggurat(r,1.0);
      //Milstein approximation:
      x += 0.5*KAPPA*KAPPA*XMIN + KAPPA*x*z + 0.5*KAPPA*KAPPA*x*(z*z-1.0);
      //Euler-Marujama approximation:
      //x += 0.5*KAPPA*KAPPA*XMIN + KAPPA*x*z

      if(x > XOVER) {x = XINIT; puts("Too large");}
      if(x < 0) {x = -x; puts("Negative");}

      delta_t = KAPPA*KAPPA/(SIGMA*SIGMA*x);

      gsl_histogram_accumulate(hist, x, delta_t);

      t_begin += delta_t; //increase distance from the beginning
    }

    t_begin -= delta;

    /* remaining part of the time step is equal to the difference of the full
     * time step and of the part of the time step outside the interval.
     * Part outside the interval is t_begin */
    sum += x*(delta_t - t_begin);
    signal[j] = sum/delta; //assign effective height to the signal.
  }

  x0 = x;
}
