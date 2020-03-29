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
NUM_PHONES_X = 3
NUM_PHONES_Y = 3
NUM_PHONES = NUM_PHONES_X * NUM_PHONES_Y 
SOUND_SPEED = 343.3          # speed of sound (in m/s)

def get_time_delay(incident_angle):
    """
    Gets time delay from user input of incident angle
    Inputs:
        incident_angle -- incident angle of sound
    Returns:
        time delay_x -- unit time delay in x direction
        time_delay_y -- unit time delay in y direction
    """
    # assert that incident angle is a number
    try:
        incident_angle = float(incident_angle)
    except ValueError:
        print('You must enter a number... exiting')
        return None, None

    t_delay_x = MIC_SEP * np.cos(np.radians(incident_angle)) / SOUND_SPEED
    t_delay_y = MIC_SEP * np.sin(np.radians(incident_angle)) / SOUND_SPEED

    return t_delay_x, t_delay_y

def gen_test_data(incident_angle):
    """
    Generates test data (to output files)
    """
    # get time delays
    t_delay_x, t_delay_y = get_time_delay(incident_angle)

    # generate time series
    t_series = np.arange(0, NUM_SAMPS * T_STEP, T_STEP)
    
    # generate mic values (that vary between 0 and 2.5 V / 0 and 512)
    fname_base = "test_data/mic_{}_ang_{}.txt"
    mic_data = np.zeros((NUM_PHONES_X, NUM_PHONES_Y, t_series.size))
    for mic_idx_x in range(NUM_PHONES_X):
        for mic_idx_y in range(NUM_PHONES_Y):
            mic_data[mic_idx_x][mic_idx_y] = 256 * (1 + np.cos(2 * np.pi * FREQ \
                                                 * (t_series + (mic_idx_x * t_delay_x) + (mic_idx_y * t_delay_y))))

            # save mic data to csv files
            mic_idx = (mic_idx_x * NUM_PHONES_Y) + mic_idx_y + 1
            with open(fname_base.format(mic_idx, incident_angle), 'w'):
                np.savetxt(fname_base.format(mic_idx, incident_angle), \
                                             mic_data[mic_idx_x][mic_idx_y], fmt='%d', delimiter=',')

if __name__ == '__main__':
    try:
        ang = float(sys.argv[1])
        gen_test_data(ang)
    except:
        for ang in np.arange(0, 360.5, 0.5):
            gen_test_data(ang)
