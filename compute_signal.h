#ifndef COMPUTE_SIGNAL_H
#define COMPUTE_SIGNAL_H

#include <gsl/gsl_rng.h>
#include <gsl/gsl_histogram.h>

void compute_signal(double * signal, const size_t length, const double delta,
    gsl_rng * r, gsl_histogram * hist);

#endif
