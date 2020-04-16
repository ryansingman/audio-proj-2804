#pragma once

#include <complex>
#include <cstring>
#include <cstdio>
#include <cmath>

// allows for multiple epochs to be sampled from same file
#define RT_MODE true
#define NUM_EPOCH_SAMPS 1024

#define NUM_SAMPS 1024
#define SAMP_RATE 9600
#define NUM_PHONES_X 3
#define NUM_PHONES_Y 3
#define NUM_PHONES (NUM_PHONES_X * NUM_PHONES_Y)
#define NUM_FFT_POINTS 256
#define NORM_FACTOR 1024

typedef double time_arr[NUM_PHONES][NUM_EPOCH_SAMPS];

#include "beamformer.h"
#include "fft.h"
#include "vis.h"

// can test single angle by setting SWEEP_START = SWEEP_END
#define SWEEP_START 0
#define SWEEP_END 360
#define SWEEP_STEP 0.5
