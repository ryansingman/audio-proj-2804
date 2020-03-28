#include "main.h"

#include <fftw3.h>

// window weights
#define A0 0.53836

typedef std::complex<double> fft_series[NUM_FFT_POINTS/2 + 1];
typedef fft_series* fft_arr[NUM_PHONES];

void fft(fft_arr *fft_data, time_arr *mic_data);
