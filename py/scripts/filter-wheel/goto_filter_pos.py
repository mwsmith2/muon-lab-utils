#!/bin/python
import sys
from muonlab.device import FilterWheel

if (len(sys.argv) < 2):
    print "Input a filter wheel position from 1-6."
    return

set_pos = int(sys.argv[1])

if (set_pos < 1 or set_pos > 6):
    print "Select a filter wheel position from 1-6."
    return

fw = FilterWheel()
fw.goto_pos(set_pos)
