#include "main.h"

#include <fftw3.h>

// window weights
#define A0 0.53836

void fft(std::complex<double> **fft_data, double **in_data);
