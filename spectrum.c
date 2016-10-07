#include "utilities.h"
#include "compute_signal.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_histogram.h>
#include <fftw3.h>
#include <stdlib.h>
#include <stdio.h>

#include "parameters.h"


int main(void){
  gsl_rng * r = init_rng();

  double * signal, * spectrum;
  allocate_arrays(&signal, &spectrum, LENGTH);

  gsl_histogram * hist = init_histogram(HIST_MIN, HIST_MAX, BIN_N);

  load_wisdom("wisdom");

  fftw_plan plan = make_plan(signal, LENGTH);

  for(size_t i = 0; i < COUNT; i++){
    printf("---- pass %zu ----\n", i);

    compute_signal(signal, LENGTH, DELTA, r, hist);
    if(i==0) output_signal("signal.dat", signal, LENGTH, DELTA, N_POINTS);

    remove_mean(signal, LENGTH);
    make_fft(plan);
    compute_spectrum(signal, spectrum, LENGTH);

    puts("Done.");
  }

  final_spectrum(spectrum, LENGTH, COUNT, DELTA, SMOOTH_NEIGHBORS);

  output_spectrum("spectrum.dat", spectrum, LENGTH, DELTA, N_POINTS);
  output_distribution("distribution.dat", hist);

  cleanup(signal, spectrum, plan, r, hist);
  return EXIT_SUCCESS;
}
