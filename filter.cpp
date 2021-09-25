#include <cstdio>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <gsl/gsl_complex.h>
#include <gsl/gsl_complex_math.h>
#include <cstring>
#include "filter.h"

void filter(double* data_in, double* data_out, struct h_filter f, double phase){

  gsl_complex p;
  
  double *data = new double[f.length];
  
  memcpy ( data, data_in, sizeof(double)*f.length );
  
  gsl_fft_real_radix2_transform(data,1,f.length);

  /*data follow gsl halfcomplex rep, filter follows "Numerical Recipes W. H. Press, S. A. Teukolsky"*/
  
  data[0] = data[0]*f.h[0];
  data[f.length/2] = data[f.length/2]*f.h[1];
  
  
  
  for(int i = 1; i < f.length/2 ; i++){
    p = gsl_complex_mul (gsl_complex_rect(data[i],data[f.length-i]),
			  gsl_complex_rect(f.h[2*i],-f.h[2*i+1]));
    p = gsl_complex_mul(p,gsl_complex_polar (1., double(i)*phase));
    data[i] = GSL_REAL(p);
    data[f.length-i] = GSL_IMAG(p);
  }

  data[0] = 0.;
  
  gsl_fft_halfcomplex_radix2_inverse(data,1,f.length);

  memcpy ( data_out, &data[f.length/4], sizeof(double)*f.length/2 );

  delete[] data;
  
}

struct of_filter load_filter(const char* filename){
  
  FILE *filter_file;

  struct of_filter f;

  f.h.name = 0;
  
  int buf_int;

  float *buf_float;
  
  printf("reading from: %s\n", filename);

  filter_file = fopen(filename, "r");

  if(!filter_file){
     printf("Error reading file\n");
     return f;
  }

  fread((char*) &buf_int, sizeof(int),1,filter_file);
  
  f.h.length = buf_int;

  printf("Size of zero padded filter: %d\n", f.h.length);

  f.h.h = new double[buf_int];
  f.std = new double[buf_int/2];
  f.nps = new double[buf_int/4+1];
  buf_float = new float[buf_int];
  
  
  fread((char*) &buf_int, sizeof(int),1,filter_file);

  printf("Filter number: %d\n", buf_int);

  fread((char*) &buf_int, sizeof(int),1,filter_file);

  f.h.name = new char[buf_int];

  fread((char*) f.h.name, sizeof(char),buf_int,filter_file);

  printf("Name of the filter: %s\n", f.h.name);

  fread((char*) buf_float, sizeof(float),f.h.length,filter_file);

  for(int i = 0; i < f.h.length;i++)
    f.h.h[i] = (double) buf_float[i];
  
  fread((char*) &buf_int, sizeof(int),1,filter_file);

  printf("Number of standard event: %d\n", buf_int);

  fread((char*) buf_float, sizeof(float),f.h.length/2,filter_file);

  for(int i = 0; i < f.h.length/2;i++)
    f.std[i] = (double) buf_float[i];

  fread((char*) buf_float, sizeof(float),f.h.length/4+1,filter_file);

  for(int i = 0; i < f.h.length/4+1;i++)
    f.nps[i] = (double) buf_float[i];

  fclose(filter_file);
  
  return f;
}
