#include "main.h"
#include "fft.h"

#define NUM_BEAMS 65

#define MIC_SEP 0.05    // distance between mics in m
#define SPEED_SOUND 343.3     // speed of sound in m/s

typedef double beam_arr[NUM_BEAMS];
typedef double time_del_arr[NUM_PHONES][NUM_BEAMS];
typedef double freq_arr[NUM_FFT_POINTS/2 + 1];
typedef std::complex<double> weight_arr[NUM_PHONES][NUM_BEAMS][NUM_FFT_POINTS/2 + 1];
typedef std::complex<double> bf_arr[NUM_BEAMS][NUM_FFT_POINTS/2 + 1];
typedef double power_arr[NUM_BEAMS];

void find_beams(beam_arr *beams);
void find_time_delays(time_del_arr *delays, beam_arr *beams);
void find_freqs(freq_arr *freqs);
void generate_weights(weight_arr *weights, freq_arr *freqs, time_del_arr *t_delays);
void beamform_data(bf_arr *bf_data, fft_arr *fft_data, weight_arr *weights);
void find_signal_beam(int *max_power_idx, bf_arr *bf_data);
