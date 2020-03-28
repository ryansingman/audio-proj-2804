import numpy as np

import matplotlib
matplotlib.use('QT5Agg')
import matplotlib.pyplot as plt

import sys

FREQ = 1000                 # signal frequency
NUM_SAMPS = 256             # number of samples to generate
NUM_FFT_POINTS = 256
SAMP_FREQ = 9600            # sampling frequency (in Hz)
T_STEP = 1 / SAMP_FREQ      # time step (in s)

MIC_SEP = 0.05                # mic separation (in m)
NUM_PHONES = 10
SOUND_SPEED = 343.3          # speed of sound (in m/s)

def get_phase_diff(incident_angle):
    """
    Gets phase angle from user input of incident angle
    Inputs:
        incident_angle -- incident angle of sound
    Returns:
        phase_diff_rad -- phase difference between mics (in radians)
    """
    # assert that incident angle is a number
    try:
        incident_angle = float(incident_angle)
    except ValueError:
        print('You must enter a number... exiting')
        return None, None

    t_delay = MIC_SEP * np.cos(np.radians(incident_angle)) / SOUND_SPEED

    phase_diff = 360 * FREQ * t_delay

    return (phase_diff * np.pi / 180.)

def display_spectrum(mic_data, incident_angle):

    # generate frequency array
    freq_series = np.arange(0, SAMP_FREQ/2, (SAMP_FREQ/(NUM_FFT_POINTS + 1)))

    # take fft
    mic_fft = np.zeros((NUM_PHONES, NUM_FFT_POINTS))
    for mic_idx in range(NUM_PHONES):
        mic_fft[mic_idx] = np.fft.rfft(mic_data[mic_idx], NUM_FFT_POINTS)

    for mic_idx in range(NUM_PHONES):
        plt.plot(freq_series, np.angle(mic_fft[mic_idx]) * 180 / np.pi, label="Mic {}".format(mic_idx+1))

    plt.title("Phase Spectrum for incident angle = {}".format(incident_angle))
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Phase (deg)')
    plt.legend()
    plt.show()

    for mic_idx in range(NUM_PHONES):
        plt.plot(freq_series, np.log10(np.abs(mic_fft[mic_idx])), label="Mic {}".format(mic_idx+1))

    plt.title("Magnitude Spectrum for incident angle = {}".format(incident_angle))
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.legend()
    plt.show()


def gen_test_data(incident_angle):
    """
    Generates test data (to output files)
    """
    # get phase difference
    phase_diff_rad = get_phase_diff(incident_angle)
    assert(phase_diff_rad is not None)

    # generate time series
    t_series = np.arange(0, NUM_SAMPS * T_STEP, T_STEP)
    
    # generate mic values (that vary between 0 and 2.5 V / 0 and 512)
    fname_base = "test_data/mic_{}_ang_{}.txt"
    mic_data = np.zeros((NUM_PHONES, t_series.size))
    for mic_idx in range(NUM_PHONES):           # assuming Uniform Line Array
        mic_data[mic_idx] = 256 * (1 + np.cos(2 * np.pi * FREQ * t_series + (mic_idx * phase_diff_rad)))

        # save mic data to csv files
        with open(fname_base.format(mic_idx+1, incident_angle), 'w'):
            np.savetxt(fname_base.format(mic_idx+1, incident_angle), mic_data[mic_idx], fmt='%d', delimiter=',')

    # display data
#    display_spectrum(mic_data, incident_angle)
    
if __name__ == '__main__':
    try:
        ang = float(sys.argv[1])
        gen_test_data(ang)
    except:
        for ang in np.arange(0, 180.5, 0.5):
            gen_test_data(ang)
