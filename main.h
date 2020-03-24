#pragma once

#include <complex>
#include <cstring>
#include <cstdio>

#define NUM_SAMPS 256
#define SAMP_RATE 9600
#define NUM_PHONES 2
#define NUM_FFT_POINTS 256
#define NORM_FACTOR 1024

#include "beamformer.h"
#include "fft.h"

#define SIGNAL_ANGLE -45

#define MIC_1_FILENAME "./test/test_data/mic_1_ang_-45.0.dat"
#define MIC_2_FILENAME "./test/test_data/mic_2_ang_-45.0.dat"

#define BOLDBLUE "\033[1;34m" 
#define RESET "\033[0m"
