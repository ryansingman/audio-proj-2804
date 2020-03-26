#include "fft.h"

void fft(std::complex<double> **fft_data, double **in_data) {

    // window in data (hamming window)
    for (int ii = 0; ii < NUM_SAMPS; ii++) {
        (*in_data)[ii] *= A0 - (1-A0)*cos((2 * M_PI * ii)/NUM_SAMPS);        
    }

    // plan and execute fft
    fftw_plan plan = fftw_plan_dft_r2c_1d(NUM_FFT_POINTS, 
                                          *in_data, 
                                           reinterpret_cast<fftw_complex *>(*fft_data), 
                                           FFTW_ESTIMATE);
    fftw_execute(plan);

    // free plan and in data
    fftw_destroy_plan(plan);
    free(*in_data);
}
