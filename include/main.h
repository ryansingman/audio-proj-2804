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

// can test single angle by setting SWEEP_START = SWEEP_END
#define SWEEP_START 45
#define SWEEP_END 135
#define SWEEP_STEP 0.5
