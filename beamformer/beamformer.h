#include "main.h"

#include <cmath>

#define NUM_BEAMS 33

#define MIC_SEP 0.1    // distance between mics in m
#define SPEED_SOUND 343     // speed of sound in m/s

typedef double time_del_arr[NUM_PHONES][NUM_BEAMS];
typedef double freq_arr[NUM_FFT_POINTS/2 + 1];
typedef std::complex<double> weight_arr[NUM_PHONES][NUM_FFT_POINTS/2 + 1][NUM_BEAMS];
typedef std::complex<double> elem_arr[NUM_PHONES][NUM_FFT_POINTS/2 + 1];
typedef std::complex<double> bf_arr[NUM_BEAMS][NUM_FFT_POINTS/2 + 1];
typedef double power_arr[NUM_BEAMS];

void find_time_delays(time_del_arr *delays);
void find_freqs(freq_arr *freqs);
void generate_weights(weight_arr *weights, freq_arr *freqs, time_del_arr *t_delays);
void beamform_data(bf_arr *bf_data, elem_arr *elem_data, weight_arr *weights);
void find_beam_powers(power_arr *power, bf_arr *bf_data);
