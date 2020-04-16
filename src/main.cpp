#include "main.h"

void read_mic_data(time_arr *mic_data, float *signal_angle, int *samp_idx) {
    /*
     * Reads microphone data from file and normalizes
     * Inputs:
     *      mic_data -- microphone data array
     *      signal_angle -- signal angle of data file
     *      samp_idx -- sample index to start read from
     */

    int read_val;

    // open file
    int string_size = 256;
    char *mic_filename = (char *) malloc(sizeof(char) * string_size);
    FILE *mic_data_file;

    for (int ii = 0; ii < NUM_PHONES; ++ii) {
        memset(mic_filename, 0, sizeof(char) * string_size);
        snprintf(mic_filename, string_size, "test/test_data/mic_%d_ang_%.1f.txt", ii+1, *signal_angle);
        mic_data_file = fopen(mic_filename, "r");

        // offset by current sample index
        fseek(mic_data_file, sizeof(int) * *samp_idx, SEEK_SET);

        for (int jj = 0; jj < NUM_EPOCH_SAMPS; jj++) {
            fscanf(mic_data_file, "%d", &read_val);
            (*mic_data)[ii][jj] = (double) read_val / (double) NORM_FACTOR;
            fscanf(mic_data_file, ",");
        }
        fclose(mic_data_file);
    }
    *samp_idx += NUM_EPOCH_SAMPS;
}

bool is_correct(float signal_angle, float truth_angle, beam_arr *beams, int max_power_idx) {
    /*
     * Determines if angle calculated matches truth angle
     * Inputs:
     *      signal_angle -- calculated signal angle
     *      truth_angle -- truth signal angle
     *      beams -- array of beam powers
     *      max_power_idx -- index of maximum power beam
     */

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
    // check for wraparound
    else if (std::abs(std::abs(truth_angle - 360) - std::abs(signal_angle)) < beam_spacing) {
        return true;
    }
    else if (std::abs(std::abs(truth_angle + 360) - std::abs(signal_angle)) < beam_spacing) {
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
    time_arr *mic_data = (time_arr *) malloc(sizeof(time_arr));

    fft_arr *fft_data = (fft_arr *) malloc(sizeof(fft_arr));
    for (int ii = 0; ii < NUM_PHONES; ++ii) {
        (*fft_data)[ii] = (fft_series *) malloc(sizeof(fft_series));
    }

    beam_arr *beams  = (beam_arr *) malloc(sizeof(beam_arr));
    time_del_arr *t_delays = (time_del_arr *) malloc(sizeof(time_del_arr));
    memset(t_delays, 0, sizeof(time_del_arr));
    freq_arr *freqs = (freq_arr *) malloc(sizeof(freq_arr));
    weight_arr *weights = (weight_arr *) malloc(sizeof(weight_arr));

    bf_arr *bf_data = (bf_arr *) malloc(sizeof(bf_arr));

    // generate weights
    find_beams(beams);
    find_time_delays(t_delays, beams);
    find_freqs(freqs);
    generate_weights(weights, freqs, t_delays);

    int max_power_idx;
    float signal_angle;

    for (float truth_angle = SWEEP_START; truth_angle <= SWEEP_END ; truth_angle += SWEEP_STEP) {

        int samp_idx = 0;

        while (samp_idx <= NUM_SAMPS - NUM_EPOCH_SAMPS) {

            // read microphone data
            memset(mic_data, 0, sizeof(time_arr));
            read_mic_data(mic_data, &truth_angle, &samp_idx);

            // perform fft on data
            fft(fft_data, mic_data);

            // beamform data
            memset(bf_data, 0, sizeof(bf_arr));
            beamform_data(bf_data, fft_data, weights);

            // find signal beam
            find_signal_beam(&max_power_idx, bf_data);

            // determine if angle is correct
            signal_angle = (*beams)[max_power_idx] * 180 / M_PI;
            bool correct = is_correct(signal_angle, truth_angle, beams, max_power_idx);

            // display angle 
            if (RT_MODE) {
//                display_angle(correct, signal_angle, truth_angle);
            }

            if (!correct) {
                printf(BOLDRED "Test failed " RESET "at truth angle: %.2f ... returned %.2f\n", truth_angle, signal_angle);
                return -1;
            }
        }
    }

    printf(BOLDGREEN "All tests passed\n" RESET);
    return 0;
}
