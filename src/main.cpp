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

bool is_correct(float signal_angle, float truth_angle, beam_arr *beams, int max_power_idx) {

    float beam_spacing;

    if (max_power_idx == 0) {
        beam_spacing = ((*beams)[1] - (*beams)[0]) * 180. / M_PI;
    }
    else if (max_power_idx == (NUM_BEAMS-1)) {
        beam_spacing = ((*beams)[NUM_BEAMS-1] - (*beams)[NUM_BEAMS-2]) * 180. / M_PI;
    }
    else {
        beam_spacing = ((*beams)[max_power_idx + 1] - (*beams)[max_power_idx - 1]) * 180. / (2 *M_PI);
    }
        
    // verify if result is correct (to closest beam)
    if (std::abs(std::abs(truth_angle) - std::abs(signal_angle)) < beam_spacing) {
        return true;
    }
    return false;

}

void display_angle(bool correct, float signal_angle, float truth_angle) {

    // visualize angle
    if (correct) {
        printf("%s", BOLDGREEN);
    }
    else printf("%s", BOLDRED);
    printf("Truth Angle: %f // Measured Angle: %f\n", truth_angle, signal_angle);
    printf("%s", RESET);
    ascii_vis(signal_angle);
}

int main(void) {

    // allocate memory up front
    double *mic_1_data = (double *) malloc(NUM_SAMPS * sizeof(double));
    double *mic_2_data = (double *) malloc(NUM_SAMPS * sizeof(double));

    std::complex<double> *mic_1_fft_data = (std::complex<double> *)malloc(sizeof(std::complex<double>)*((NUM_FFT_POINTS / 2) + 1));
    std::complex<double> *mic_2_fft_data = (std::complex<double> *)malloc(sizeof(std::complex<double>)*((NUM_FFT_POINTS / 2) + 1));

    elem_arr *elem_data = (elem_arr *) malloc(sizeof(elem_arr));;

    beam_arr *beams  = (beam_arr *) malloc(sizeof(beam_arr));
    time_del_arr *t_delays = (time_del_arr *) malloc(sizeof(time_del_arr));
    freq_arr *freqs = (freq_arr *) malloc(sizeof(freq_arr));
    weight_arr *weights = (weight_arr *) malloc(sizeof(weight_arr));

    bf_arr *bf_data = (bf_arr *) malloc(sizeof(bf_arr));

    int max_power_idx;
    float signal_angle;

    int string_size = 256;
    char *mic_1_filename = (char *) malloc(sizeof(char) * string_size);
    char *mic_2_filename = (char *) malloc(sizeof(char) * string_size);

    for (float truth_angle = SWEEP_START; truth_angle <= SWEEP_END ; truth_angle += SWEEP_STEP) {

        // read mic 1 data into array
        memset(mic_1_filename, 0, sizeof(char) * string_size);
        snprintf(mic_1_filename, string_size, "test/test_data/mic_1_ang_%.1f.txt", truth_angle);
        FILE *mic_1_data_file = fopen(mic_1_filename, "r");
        read_mic_data(&mic_1_data, mic_1_data_file);

        // read mic 2 data into array
        memset(mic_2_filename, 0, sizeof(char) * string_size);
        snprintf(mic_2_filename, string_size, "test/test_data/mic_2_ang_%.1f.txt", truth_angle);
        FILE *mic_2_data_file = fopen(mic_2_filename, "r");
        read_mic_data(&mic_2_data, mic_2_data_file);

        // perform fft on data
        fft(&mic_1_fft_data, &mic_1_data);
        fft(&mic_2_fft_data, &mic_2_data);

        // place fft data in elem arr
        memcpy((*elem_data)[0], mic_1_fft_data, sizeof(elem_arr) / 2);
        memcpy((*elem_data)[1], mic_2_fft_data, sizeof(elem_arr) / 2);

        // generate weights
        memset(t_delays, 0, sizeof(time_del_arr));
        memset(weights, 1, sizeof(weight_arr));
        
        find_beams(beams);
        find_time_delays(t_delays, beams);
        find_freqs(freqs);
        generate_weights(weights, freqs, t_delays);

        // beamform data
        beamform_data(bf_data, elem_data, weights);

        // find signal beam
        find_signal_beam(&max_power_idx, bf_data);

        // determine if angle is correct
        signal_angle = (*beams)[max_power_idx] * 180 / M_PI;
        bool correct = is_correct(signal_angle, truth_angle, beams, max_power_idx);

        // display angle 
//        display_angle(correct, signal_angle, truth_angle);

        if (!correct) {
            printf(BOLDRED "Test failed at angle: %.2f ... returned %.2f\n" RESET, truth_angle, signal_angle);
            return -1;
        }
        else {
            printf(BOLDGREEN "Test passed at truth angle: %.2f ... returned %.2f\n" RESET, truth_angle, signal_angle);
        }

    }

    return 0;
}
