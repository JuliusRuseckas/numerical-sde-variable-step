#include <fftw3.h>
#include <stdio.h>
#include <stdlib.h>

#define LENGTH 1048576u

int main(void){

  double * signal = fftw_malloc(LENGTH*sizeof(double));
  if(!signal){
    perror("Error allocating memory");
    return EXIT_FAILURE;
  }

  puts("Making plan ...");
  fftw_plan plan = fftw_plan_r2r_1d(LENGTH, signal, signal, FFTW_R2HC, FFTW_PATIENT);
  puts("Done.");
  fftw_print_plan(plan);
  puts("------------");

  const char *filename = "wisdom";
  FILE * f = fopen(filename, "w");
  if(!f){
    perror("Error opening file");
    return EXIT_FAILURE;
  }
  printf("Writing wisdom to file %s ...\n", filename);

  fftw_export_wisdom_to_file(f);

  fclose(f);

  fftw_free(signal);
  fftw_destroy_plan(plan);
  fftw_cleanup();
  return EXIT_SUCCESS;
}
