import numpy as np

import matplotlib
matplotlib.use('QT5Agg')
import matplotlib.pyplot as plt

FREQ = 1000                 # signal frequency
NUM_SAMPS = 256             # number of samples to generate
NUM_FFT_POINTS = 256
NUM_BEAMS = 33
SAMP_FREQ = 9600            # sampling frequency (in Hz)
T_STEP = 1 / SAMP_FREQ      # time step (in s)

MIC_SEP = 0.1                # mic separation (in m)
SOUND_SPEED = 343             # speed of sound (in m/s)

def get_phase_diff():
    """
    Gets phase angle from user input of incident angle
    Returns:
        incident_angle -- incident angle of sound
        phase_diff_rad -- phase difference between mics (in radians)
    """
    # get incident angle from user input
    incident_angle = input('Enter phase angle (in degrees): ')

    # assert that incident angle is a number
    try:
        incident_angle = float(incident_angle)
    except ValueError:
        print('You must enter a number... exiting')
        return None, None

    t_delay = MIC_SEP * np.cos(np.radians(incident_angle)) / SOUND_SPEED

    print(t_delay)
    phase_diff = 360 * FREQ * t_delay

    return incident_angle, (phase_diff * np.pi / 180.)

def beamform(mic_1, mic_2):

    # take fft
    mic_1_fft = np.fft.rfft(mic_1, NUM_FFT_POINTS)
    mic_2_fft = np.fft.rfft(mic_2, NUM_FFT_POINTS)

    # generate weights
    t_delays = np.cos(np.radians(np.arange(0, 180, 180./NUM_BEAMS))) * MIC_SEP * SOUND_SPEED
    freq_series = np.arange(0, SAMP_FREQ/2, (SAMP_FREQ/(NUM_FFT_POINTS + 1)))
    weights = np.exp(1j * 2 * np.pi * freq_series * t_delays)

    # beamform 
    bf_data = np.zeros((NUM_BEAMS, NUM_FFT_POINTS))

    plt.plot(freq_series, np.abs(mic_1_fft) * 180 / np.pi)
    plt.plot(freq_series, np.abs(mic_2_fft) * 180 / np.pi)
    plt.show()


def gen_test_data():
    """
    Generates test data (to output files)
    """
    # get phase difference
    incident_angle, phase_diff_rad = get_phase_diff()
    assert(phase_diff_rad is not None)
    print(phase_diff_rad)

    # generate time series
    t_series = np.arange(0, NUM_SAMPS * T_STEP, T_STEP)
    
    # generate mic values (that vary between 0 and 2.5 V / 0 and 512)
    mic_1 = 256 * np.cos(2 * np.pi * FREQ * t_series)
    mic_2 = 256 * np.cos(2 * np.pi * FREQ * t_series + phase_diff_rad)

    # python beamformer
#    beamform(mic_1, mic_2)

    # save mic data to csv files
    fname_base = "test_data/mic_{}_ang_{}.dat"
    with open(fname_base.format(1, incident_angle), 'w'):
        np.savetxt(fname_base.format(1, incident_angle), mic_1, fmt='%d', delimiter=',')
    with open(fname_base.format(2, incident_angle), 'w'):
        np.savetxt(fname_base.format(2, incident_angle), mic_2, fmt='%d', delimiter=',')
    
if __name__ == '__main__':
    gen_test_data()
