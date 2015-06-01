#!/bin/python

import time, sys, u3, subprocess

start_pos = 6
current_pos = start_pos

if (len(sys.argv) < 2):
    print "Select a filter wheel position from 1-6."

else:

    set_pos = int(sys.argv[1])
    if (set_pos < 1 or set_pos > 6):
        print "Select a filter wheel position from 1-6."

    d = u3.U3()

    while (current_pos != set_pos):

        d.setDOState(10, 1)
        time.sleep(0.5)

        d.setDOState(10, 0)
        time.sleep(0.5)

        current_pos = (current_pos % 6) + 1
        print "The filter wheel is now at position %i." % (current_pos)

    os.chdir(os.path.dirname(os.path.realpath(__file__)))
    sub_string = 's/start_step = %i/start_step = %i/g' % (start_pos, current_pos)
    subprocess.call(['sed', '-i', sub_string , __file__])

