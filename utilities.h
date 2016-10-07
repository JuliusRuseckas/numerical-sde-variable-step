#ifndef UTILITIES_H
#define UTILITIES_H

#include <gsl/gsl_rng.h>
#include <gsl/gsl_histogram.h>
#include <fftw3.h>

gsl_rng * init_rng(void);

void allocate_arrays(double ** signal, double ** spectrum,
    const size_t length);

gsl_histogram * init_histogram(const double r_min, const double r_max,
    const size_t bin_n);

gsl_histogram * init_histogram_uniform(const double r_min, const double r_max,
    const size_t bin_n);

void load_wisdom(const char * filename);

fftw_plan make_plan(double * signal, const size_t length);

void make_fft(fftw_plan plan);

void remove_mean(double * signal, const size_t length);

void compute_spectrum(const double * signal, double * spectrum,
    const size_t length);

void smooth(const unsigned neighbors, double *spectrum, const size_t length);

void final_spectrum(double *spectrum, const size_t length,
    const unsigned count, const double delta, const unsigned neighbors);

double frequency(const size_t i, const size_t length, const double delta);

void output_spectrum(const char * filename, const double * spectrum,
    const size_t length, const double delta, const size_t n_points);

void output_signal(const char * filename, const double * signal,
    const size_t length, const double delta, const size_t n_points);

void output_signal_full(const char * filename, const double * signal,
    const size_t length, const double delta);

void output_distribution(const char * filename, const gsl_histogram * hist);

void cleanup(double * signal, double * spectrum, fftw_plan plan, gsl_rng * r,
    gsl_histogram * hist);

#endif
