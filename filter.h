#ifndef FILTER_H
#define FILTER_H

struct h_filter{
  char* name; //name of the filter
  double *h; //filter itself (half complex format
  int length; //record length
  };

struct of_filter{
  struct h_filter h; //zero padded filter
  double *std; //std pulse (no zero padding)
  double *nps; //noise power spectrum
};

struct of_filter load_filter(const char* filename);
void filter(double* data_in, double* data_out, struct h_filter f, double phase = 0.);

#endif