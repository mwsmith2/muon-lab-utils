#!/bin/python
from muonlab.device import UvaBias

# Read out the voltages in the high voltage supply.
hv = UvaBias()
hv.read_all_channels()
