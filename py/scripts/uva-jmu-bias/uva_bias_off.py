#!/bin/python
from muonlab.device import UvaBias

# Turn off and set the voltages.
hv = UvaBias('http://192.168.1.50')
hv.bias_off()

# Read out the voltages to make sure they were set
hv.read_all_channels()
