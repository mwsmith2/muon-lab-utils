import u3
import time

d = u3.U3()

d.setDOState(10, 1)

time.sleep(0.5)

d.setDOState(10, 0)


