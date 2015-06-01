#!/bin/python

import sys
import slow_control

usage_warning = "Usage: \npython check_bias_voltage.py <device_path1> <(opt)device_path2>"

# Check for the right number of arguments
if (len(sys.argv) < 2):

    print usage_warning

# Check voltages
for i in range(1, len(sys.argv)):

    bk = slow_control.BKPrecision(sys.argv[i])
    
    print ""

    print sys.argv[i] + ' voltage: ' + str(bk.meas_volt())

    if (bk.id == 1006):
        print sys.argv[i] + ' is connected to the white wrapped crystals.'

    elif (bk.id == 0004):
        print sys.argv[i] + ' is connected to the black wrapped crystals.'

    print ""

