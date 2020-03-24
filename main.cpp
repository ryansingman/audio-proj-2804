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

void display_power(power_arr *power) {

    float angle = 0;

    // find max power
    float max = 0;
    for (int ii = 0; ii < NUM_BEAMS; ++ii) {
        if ((*power)[ii] > max) {
            max = (*power)[ii];
            angle = 180 * ((float)(NUM_BEAMS - 1 - ii) / (float)(NUM_BEAMS - 1));
        }
    }

    // verify if result is correct (to closest beam)
    bool correct = false;
    float beam_spacing = 180. / (float)NUM_BEAMS;
    if (std::abs(std::abs(TRUTH_SIGNAL_ANGLE) - std::abs(angle)) < beam_spacing) {
        correct = true;
    }

    // visualize angle
    for (int jj = 0; jj < (NUM_BEAMS); ++jj) {
        if ((*power)[jj] == max) {
            if (correct) {
                printf("%s", BOLDGREEN);
            }
            else printf("%s", BOLDRED);
            printf("Truth Angle: %f // Measured Angle: %f\n", (float)TRUTH_SIGNAL_ANGLE, angle);
            printf("%s", RESET);
            ascii_vis(&angle);
        }
    }
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
    elem_arr elem_data = { {0} };
    memcpy(elem_data[0], mic_1_fft_data, sizeof(elem_arr) / 2);
    memcpy(elem_data[1], mic_2_fft_data, sizeof(elem_arr) / 2);

    // generate weights
    time_del_arr t_delays =   { {0} };
    freq_arr freqs        =     {0};
    weight_arr weights    = { { {1} } };
    
    find_time_delays(&t_delays);
    find_freqs(&freqs);
    generate_weights(&weights, &freqs, &t_delays);

    // beamform data
    bf_arr bf_data = { {0} };
    beamform_data(&bf_data, &elem_data, &weights);

    // find beam powers
    power_arr power = {0};
    find_beam_powers(&power, &bf_data);

    // display beam powers 
    display_power(&power);

    return 0;
}
