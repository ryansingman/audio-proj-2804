#pragma once

#include <complex>
#include <cstring>
#include <cstdio>
#include <cmath>

#define NUM_SAMPS 256
#define SAMP_RATE 9600
#define NUM_PHONES 2
#define NUM_FFT_POINTS 256
#define NORM_FACTOR 1024

#include "beamformer.h"
#include "fft.h"
#include "vis.h"

#define SIGNAL_ANGLE 90

#define MIC_1_FILENAME "./test/test_data/mic_1_ang_90.0.txt"
#define MIC_2_FILENAME "./test/test_data/mic_2_ang_90.0.txt"
