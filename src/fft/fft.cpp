#include "fft.h"

void fft(fft_arr *fft_data, time_arr *mic_data) {

    for (int ii = 0; ii < NUM_PHONES; ++ii) {

        // window in data (hamming window)
        for (int jj = 0; jj < NUM_EPOCH_SAMPS; ++jj) {
            (*mic_data)[ii][jj] *= A0 - (1-A0)*cos((2 * M_PI * jj)/NUM_EPOCH_SAMPS); 
        }

        fftw_plan plan = fftw_plan_dft_r2c_1d(NUM_FFT_POINTS, 
                                              (*mic_data)[ii], 
                                              reinterpret_cast<fftw_complex *>(*(*fft_data)[ii]),
                                              FFTW_ESTIMATE);

        // plan fft
        fftw_execute(plan);

        // free plan
        fftw_destroy_plan(plan);

    }

}
