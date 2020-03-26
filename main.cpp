#include "main.h"

void read_mic_data(double **mic_data, FILE *mic_data_file) {
    /*
     * Reads microphone data from file and normalizes
     * Inputs:
     *      mic_data -- microphone data array
     *      mic_data_file -- microphone data file
     */

    int read_val;
    for (int ii = 0; ii < NUM_SAMPS; ii++) {
        fscanf(mic_data_file, "%d", &read_val);
        (*mic_data)[ii] = (double) read_val / (double) NORM_FACTOR;
        fscanf(mic_data_file, ",");
    }
}

void display_angle(beam_arr *beams, int max_power_idx) {

    float angle = (*beams)[max_power_idx] * 180 / M_PI;
    float beam_spacing;

    if (max_power_idx == 0) {
        beam_spacing = ((*beams)[1] - (*beams)[0]) * 180. / M_PI;
    }
    else if (max_power_idx == NUM_BEAMS) {
        beam_spacing = ((*beams)[NUM_BEAMS] - (*beams)[NUM_BEAMS - 1]) * 180. / M_PI;
    }
    else {
        beam_spacing = ((*beams)[max_power_idx + 1] - (*beams)[max_power_idx - 1]) * 180. / (2 *M_PI);
    }
        
    // verify if result is correct (to closest beam)
    bool correct = false;
    if (std::abs(std::abs(TRUTH_SIGNAL_ANGLE) - std::abs(angle)) < beam_spacing) {
        correct = true;
    }

    // visualize angle
    if (correct) {
        printf("%s", BOLDGREEN);
    }
    else printf("%s", BOLDRED);
    printf("Truth Angle: %f // Measured Angle: %f\n", (float)TRUTH_SIGNAL_ANGLE, angle);
    printf("%s", RESET);
    ascii_vis(&angle);
}

int main(void) {

    // read mic 1 data into array
    double *mic_1_data = (double *) malloc(NUM_SAMPS * sizeof(double));
    FILE *mic_1_data_file = fopen(MIC_1_FILENAME, "r");
    read_mic_data(&mic_1_data, mic_1_data_file);

    // read mic 2 data into array
    double *mic_2_data = (double *) malloc(NUM_SAMPS * sizeof(double));
    FILE *mic_2_data_file = fopen(MIC_2_FILENAME, "r");
    read_mic_data(&mic_2_data, mic_2_data_file);

    // perform fft on data
    std::complex<double> *mic_1_fft_data = (std::complex<double> *)malloc(sizeof(std::complex<double>)*((NUM_FFT_POINTS / 2) + 1));
    fft(&mic_1_fft_data, &mic_1_data);
    std::complex<double> *mic_2_fft_data = (std::complex<double> *)malloc(sizeof(std::complex<double>)*((NUM_FFT_POINTS / 2) + 1));
    fft(&mic_2_fft_data, &mic_2_data);

    // place fft data in elem arr
    elem_arr *elem_data = (elem_arr *) malloc(sizeof(elem_arr));;
    memset(elem_data, 0, sizeof(elem_arr));
    memcpy((*elem_data)[0], mic_1_fft_data, sizeof(elem_arr) / 2);
    memcpy((*elem_data)[1], mic_2_fft_data, sizeof(elem_arr) / 2);

    // generate weights
    beam_arr *beams  = (beam_arr *) malloc(sizeof(beam_arr));
    time_del_arr *t_delays = (time_del_arr *) malloc(sizeof(time_del_arr));
    memset(t_delays, 0, sizeof(time_del_arr));
    freq_arr *freqs = (freq_arr *) malloc(sizeof(freq_arr));
    weight_arr *weights = (weight_arr *) malloc(sizeof(weight_arr));
    memset(weights, 1, sizeof(weight_arr));
    
    find_beams(beams);
    find_time_delays(t_delays, beams);
    find_freqs(freqs);
    generate_weights(weights, freqs, t_delays);

    // beamform data
    bf_arr *bf_data = (bf_arr *) malloc(sizeof(bf_arr));
    beamform_data(bf_data, elem_data, weights);

    // find signal beam
    int max_power_idx;
    find_signal_beam(&max_power_idx, bf_data);

    // display angle 
    display_angle(beams, max_power_idx);

    return 0;
}
