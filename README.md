# Audio Direction Finder

> C++ Frequency-Domain Beamformer

## How does this work?

This beamformer uses frequency-domain beamforming to find the direction of highest energy. 

To perform frequency-domain beamforming, you must first transform the (time domain audio) signal to the frequency domain. In this implementation, the transformation is completed using FFTW3, an open-source FFT (fast fourier transform) library. 

After the signal is placed in the frequency domain, each bin is multiplied by a complex exponential that phase shifts the bin. The cummulative result of the phase shifts is a transformation to an alternate domain: the **beam-frequency** domain. This domain differs from the general frequency domain as there are now frequency domain representations of the signal for every **look direction**, rather than every microphone input.

## Flexibility

In designing this beamformer, great emphasis was placed on flexibility. With simple changes to `main.h`, this beamformer can be made to work with an arbitrary (>1) number of microphones, number of beams, as well as sample rate and microphone spacing (though the beamformer does assume linear, 1-dimensional microphone spacing).

## How do I run this beamformer?

The first step to running the beamformer is to generate test data. To generate test data, follow the following steps:

    1. `cd test_data`
    2. `python3 test_data_gen.py
    3. Enter your desired incident angle

> Note: you must have python3 installed on your machine, along with NumPy and Matplotlib (with a QT5 backend)

After generating test data, ensure that you are pointing to the correct file in `main.h`. To do so, check that the filepath under `MIC_1_FILENAME` and `MIC_2_FILENAME` are correct.

Next, build the project. You can do so by running the command `make build` in the top-level directory. You must have FFTW3 built in order to build the project.

Finally, you can run the project. This is done by running the command `make run`.
