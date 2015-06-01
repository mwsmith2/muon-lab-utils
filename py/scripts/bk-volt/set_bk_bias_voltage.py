#!/bin/python

import sys
import slow_control

usage_warning = "Usage: \npython set_bias_voltage.py <voltage> <device_path1> <(opt)device_path2>"

# Check for the right number of arguments
if (len(sys.argv) < 3):
    print usage_warning

# Check if the arguments are proper order
try:
    volt = float(sys.argv[1])

except:
    print usage_warning
    

# Change voltages
for i in range(2, len(sys.argv)):

    bk = slow_control.BKPrecision(sys.argv[i])
    last_volt = bk.meas_volt()
    bk.set_volt(volt)
    print sys.argv[i] + ' voltage was ' + str(last_volt) + 'V.'
    print 'It is now ' + str(bk.meas_volt()) + 'V.'
