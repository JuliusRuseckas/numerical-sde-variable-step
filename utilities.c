#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void print_error_msg(const char * msg);

gsl_rng * init_rng(void){
  time_t seed;
  time(&seed);

  gsl_rng_env_setup();
  gsl_rng * r = gsl_rng_alloc(gsl_rng_default);
  gsl_rng_set(r, (unsigned)seed);
  return r;
}


void allocate_arrays(double ** signal, double ** spectrum,
    const size_t length){

  *signal = fftw_malloc(length*sizeof(double));
  if(!(*signal)) print_error_msg("Error allocating memory");

  size_t len2 = length/2 + 1;

  *spectrum = malloc(len2*sizeof(double));
  if(!(*spectrum)) print_error_msg("Error allocating memory");

  for(size_t j = 0; j < len2; j++)
    (*spectrum)[j] = 0.0;
}


gsl_histogram * init_histogram(const double r_min, const double r_max,
    const size_t bin_n){

  double * ranges = malloc((bin_n+1) * sizeof(double));
  if(!ranges) print_error_msg("Error allocating memory");

  gsl_histogram * hist = gsl_histogram_alloc(bin_n);
  if(!hist) print_error_msg("Error allocating histogram");

  double ratio = pow(r_max/r_min, 1.0/(double)bin_n);
  double current_range = r_min;
  for(size_t j = 0; j <= bin_n; j++){
    ranges[j] = current_range;
    current_range *= ratio;
  }

  gsl_histogram_set_ranges(hist, ranges, bin_n+1);

  free(ranges);
  return hist;
}


gsl_histogram * init_histogram_uniform(const double r_min, const double r_max,
    const size_t bin_n){

  gsl_histogram * hist = gsl_histogram_alloc(bin_n);
  if(!hist) print_error_msg("Error allocating histogram");

  gsl_histogram_set_ranges_uniform(hist, r_min, r_max);
  return hist;
}


void load_wisdom(const char * filename){
  FILE * f = fopen(filename, "r");
  if(!f) print_error_msg("Error openning wisdom file");

  printf("Loading wisdom from file %s ...\n", filename);

  int ret = fftw_import_wisdom_from_file(f);
  if (ret != 1) print_error_msg("Error importing wisdom");

  fclose(f);
}


fftw_plan make_plan(double * signal, const size_t length){
  puts("Making plan ...");

  fftw_plan plan = fftw_plan_r2r_1d((int)length, signal, signal,
      FFTW_R2HC, FFTW_MEASURE);
  puts("Done.");
  fftw_print_plan(plan);
  puts("");
  return plan;
}


void make_fft(fftw_plan plan){
  puts("Making FFT ...");
  fftw_execute(plan);
}


void remove_mean(double * signal, const size_t length){
  puts("Removing mean ...");

  double mean = 0.0;
  for(size_t i = 0; i < length; i++)
    mean += (signal[i] - mean) / (double)(i+1);

  for(size_t i = 0; i < length; i++)
    signal[i] = signal[i] - mean;
}


void compute_spectrum(const double * signal,  double * spectrum,
    const size_t length){

  puts("Calculating spectrum ...");

  double re = signal[0];
  spectrum[0] += re*re;

  size_t len2 = length/2;

  re = signal[len2];
  spectrum[len2] += re*re;

  for(size_t i = 1; i < len2; i++){
    re = signal[i];
    double im = signal[length-i];
    spectrum[i] += re*re + im*im;
  }
}


void smooth(const unsigned neighbors, double *spectrum, const size_t length){

  double * smooth_spectrum = malloc(length*sizeof(double));
  if(!smooth_spectrum) print_error_msg("Error allocating memory");

  puts("Making smooth ...");
  for(size_t i = 0; i < length; i++)
    smooth_spectrum[i] = 0.0;

  for(size_t i = 0; i < length; i++){
    size_t n_neighbors = 0;
    for (size_t j = (i < neighbors ? 0 : i-neighbors); j <= i+neighbors; j++)
      if (j < length) {
        smooth_spectrum[i] += spectrum[j];
        n_neighbors++;
      }
    smooth_spectrum[i] = smooth_spectrum[i]/(double)n_neighbors;
  }

  for(size_t i = 0; i < length; i++)
    spectrum[i] = smooth_spectrum[i];

  free(smooth_spectrum);
}


void final_spectrum(double *spectrum, const size_t length,
    const unsigned count, const double delta, const unsigned neighbors){

  size_t len2 = length/2 + 1;
  for(size_t j = 0; j < len2; j++)
    spectrum[j] *= 2.0*delta/((double)length*count);

  smooth(neighbors, spectrum, len2);
}


double frequency(const size_t i, const size_t length, const double delta){
  if(i <= length/2)
    return (double)i/(delta*(double)length);
  else
    return ((double)i-(double)length)/(delta*(double)length);
}


/* reduce the number of the points to n_points and
 * write the spectrum to a file  */
void output_spectrum(const char * filename, const double * spectrum,
    const size_t length, const double delta, const size_t n_points){

  FILE * f = fopen(filename, "w");
  if(!f) print_error_msg("Error opening file");
  printf("Writing to file %s ...\n", filename);

  /* the ratio of maximal and minimal frequencies is equal to length */
  double ratio = pow(0.5*(double)length, 1.0/(double)n_points);
  printf("ratio=%g\n", ratio);

  /* j=0, omega=0 does not make sense in the logaritmic scale */
  double position = 1.0;
  /* when j>length/2 then we have negative frequencies. Does not make sense in
   * logaritmic scale */
  while(position <= 0.5*(double)length){
    size_t j = (size_t)floor(position);
    fprintf(f, "%g\t%g\n", frequency(j, length, delta), spectrum[j]);
    position *= ratio;
  }

  fclose(f);
}


/* reduce the number of the points to n_points and
 * output signal to a file */
void output_signal(const char * filename, const double * signal,
    const size_t length, const double delta, const size_t n_points){

  FILE * f = fopen(filename, "w");
  if(!f) print_error_msg("Error opening file");
  printf("Writing signal to file %s ...\n", filename);

  size_t len = (n_points < length) ? n_points : length;

  for(size_t j = 0; j < len; j++)
    fprintf(f, "%g\t%g\n", (double)j*delta, signal[j]);

  fclose(f);
}


void output_signal_full(const char * filename, const double * signal,
    const size_t length, const double delta){

  FILE * f = fopen(filename, "w");
  if(!f) print_error_msg("Error opening file");
  printf("Writing signal to file %s ...\n", filename);

  for(size_t j = 0; j < length; j++)
    fprintf(f, "%g\t%g\n", (double)j*delta, signal[j]);

  fclose(f);
}


void output_distribution(const char * filename, const gsl_histogram * hist){
  FILE * f = fopen(filename, "w");
  if(!f) print_error_msg("Error opening file");
  printf("Writing distribution to file %s ...\n", filename);

  double sum = gsl_histogram_sum(hist);

  for(size_t j = 0; j < hist->n; j++){
    double r1 = hist->range[j];
    double r2 = hist->range[j+1];
    fprintf(f, "%g\t%g\n", (r1+r2)*0.5, (hist->bin[j])/((r2-r1)*sum));
  }

  fclose(f);
}


void cleanup(double * signal, double * spectrum, fftw_plan plan, gsl_rng * r,
    gsl_histogram * hist){

  fftw_free(signal);
  free(spectrum);
  gsl_rng_free(r);
  gsl_histogram_free(hist);
  fftw_destroy_plan(plan);
  fftw_cleanup();
}


void print_error_msg(const char * msg){
  perror(msg);
  exit(EXIT_FAILURE);
}
