#include "beamformer.h"

void find_time_delays(time_del_arr *delays) {

    double angle, angleRad;

    // iterate thru phones
    for (int ii = 1; ii < NUM_PHONES; ++ii) {
        // iterate thru beams
        for (int jj = 0; jj < NUM_BEAMS; ++jj) {
            angle = -90 + (180 * ((double)jj / (double)(NUM_BEAMS - 1)));
            angleRad = M_PI * (double)angle / (double)180;

            (*delays)[ii][jj] = (double) (ii * MIC_SEP * sin(angleRad)) / (double)SPEED_SOUND;
        }
    }
}

void find_freqs(freq_arr *freqs) {

    // find frequency step
    double freq_step = ((SAMP_RATE) / (NUM_FFT_POINTS - 1));

    // find each frequency
    for (int ii = 0; ii < (NUM_FFT_POINTS/2) + 1; ++ii) {
        (*freqs)[ii] = (double)ii * freq_step;
    }
}

void generate_weights(weight_arr *weights, freq_arr *freqs, time_del_arr *t_delays) {

    for (int ii = 0; ii < NUM_PHONES; ++ii) {
        for (int jj = 0; jj < NUM_BEAMS; ++jj) {
            for (int kk = 0; kk < (NUM_FFT_POINTS/2) + 1; ++kk) {
                (*weights)[ii][jj][kk] = std::exp(std::complex<double>(0.0, -2 * M_PI * (*freqs)[kk] * (*t_delays)[ii][jj]));
            }
        }
    }
}

void beamform_data(bf_arr *bf_data, elem_arr *elem_data, weight_arr *weights) {
   
    // iterate thru phones
    for (int ii = 0; ii < NUM_PHONES; ++ii) {
        // iterate thru beams
        for (int jj = 0; jj < NUM_BEAMS; ++jj) {
            // iterate thru fft points
            for (int kk = 0; kk < (NUM_FFT_POINTS/2) + 1; ++kk) { 
                (*bf_data)[jj][kk] += (*weights)[ii][jj][kk] * (*elem_data)[ii][kk];
           }
        }
    }
}

void find_beam_powers(power_arr *power, bf_arr *bf_data) {

    double max_power = 0;
    // iterate thru beams
    for (int ii = 0; ii < NUM_BEAMS; ++ii) {
        // iterate thru fft points
        for (int jj = 0; jj < (NUM_FFT_POINTS/2) + 1; ++jj) {
            (*power)[ii] += std::pow(std::abs((*bf_data)[ii][jj]), 2);
        }
        if (max_power < (*power)[ii]) {
            max_power = (*power)[ii];
        }
    }

    // normalize power
    for (int ii = 0; ii < NUM_BEAMS; ++ii) {
        (*power)[ii] /= max_power;
    }
}
