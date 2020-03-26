#include "beamformer.h"

void find_beams(beam_arr *beams) {
    /*
     * Generates array of beam angles
     * Inputs:
     *   beams -- beam array to fill in
     */

    for (int ii = 0; ii < NUM_BEAMS; ++ii) {
        (*beams)[ii] = acos((double)(NUM_BEAMS/2 - ii) / (double)((NUM_BEAMS - 1)/2)); // angle vector is linear in cosine space
    }
}

void find_time_delays(time_del_arr *delays, beam_arr *beams) {
    /*
     * Generates array of time delays
     * Inputs:
     *   delays -- time delay array to fill in
     *   beams -- array of beam angles 
     */

    double angle;

    // iterate thru phones
    for (int ii = 1; ii < NUM_PHONES; ++ii) {
        // iterate thru beams
        for (int jj = 0; jj < NUM_BEAMS; ++jj) {
            angle = (*beams)[jj];

            (*delays)[ii][jj] = (double) (ii * MIC_SEP * cos(angle)) / (double)SPEED_SOUND;
        }
    }
}

void find_freqs(freq_arr *freqs) {
    /*
     * Generates array of (angular) frequencies
     * Inputs:
     *   freqs -- frequency array to fill in
     */

    // find frequency step
    double freq_step = 2 * M_PI * ((SAMP_RATE) / (NUM_FFT_POINTS - 1));

    // find each frequency
    for (int ii = 0; ii < (NUM_FFT_POINTS/2) + 1; ++ii) {
        (*freqs)[ii] = (double)ii * freq_step;
    }
}

void generate_weights(weight_arr *weights, freq_arr *freqs, time_del_arr *t_delays) {
    /*
     * Generates array of complex exponential weights
     * Inputs:
     *   weights-- weight array to fill in
     *   freqs -- array of angular frequencies
     *   t_delays -- array of time delays
     */

    // iterate thru phones
    for (int ii = 0; ii < NUM_PHONES; ++ii) {
        // iterate thru beams
        for (int jj = 0; jj < NUM_BEAMS; ++jj) {
            // iterate thru fft points
            for (int kk = 0; kk < (NUM_FFT_POINTS/2) + 1; ++kk) { 
                (*weights)[ii][jj][kk] = std::exp(std::complex<double>(0.0, -1 * (*freqs)[kk] * (*t_delays)[ii][jj]));
            }
        }
    }
}

void beamform_data(bf_arr *bf_data, elem_arr *elem_data, weight_arr *weights) {
    /*
     * Beamforms data using complex exponential phase shifting
     * Inputs:
     *   bf_data -- array of beamformed data to fill in
     *   elem_data -- element data (in frequency-domain)
     *   weights -- weight array of complex exponential weights
     */
   
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

void find_signal_beam(int *max_power_idx, bf_arr *bf_data) {
    /*
     * Finds beam index of highest power beam
     * Inputs:
     *   max_power_idx -- index of max power beam (to be filled in)
     *   bf_data -- array of beamformed data
     */

    double max_power = 0, power;
    // iterate thru beams
    for (int ii = 0; ii < NUM_BEAMS; ++ii) {
        power = 0;
        // iterate thru fft points
        for (int jj = 0; jj < (NUM_FFT_POINTS/2) + 1; ++jj) {
            power += std::pow(std::abs((*bf_data)[ii][jj]), 2);
        }
        if (max_power < power) {
            max_power = power;
            *max_power_idx = ii;
        }
    }
}
