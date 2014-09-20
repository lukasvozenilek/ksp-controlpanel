#Created by Lukas Vozenilek
#
#
#

from __future__ import division
from math import *
import time
import modules.telemachus_plugin as tele
from modules.check_stage import check_stage

#Ask for orbit height




tele.sas(1)
tele.throttle_full()
for x in range(10):
    print "T-" + str(10-x)
    if 10-x == 2:
        print("Ignition sequence")
        tele.stage()
    time.sleep(1)
print("T-0")
print("Releasing launch clamps")
tele.stage()


time.sleep(2)
cur2 = 1
#wait until gravity turn altitude while monitoring ascent
while (1):
    curAsl = tele.read_asl()
    if curAsl > 10000 and cur == 1:
        print("Reached 10km, initiating gravity turn")
        tele.MJ_att2(90,50,90)
        cur = 2
    elif tele.read_apoapsis() > orbit_height and cur == 2:
        print("Ap reached " + str(orbit_height) + " km")
        tele.throttle_zero()
        break
    
    #Auto-abort system
    VS = tele.read_verticalspeed()
    if (VS < 50 and curAsl > 1000):
        print("Vertical speed decreasing - ABORT")
        tele.abort()
        tele.sas(0)
        exit()
    check_stage(tele)
    time.sleep(0.1)


#orbital insertion
print("Changing attitude")
tele.rcs(1)
tele.MJ_pro()



print("Please use KER or MECHJEB to enter these values:")
cur_mass = float(raw_input("Mass in Mg: "))
cur_thrust = float(raw_input("Thrust in Kn: "))

print("Calculating burn time")
burn_time = (0.9 * 1650 * (cur_mass/cur_thrust)) - 5


print("Burn time is " + str(burn_time) + " seconds")
TTAP = burn_time/2
print("Waiting for Pe-" + str(TTAP))
while tele.read_time_to_ap() > TTAP:
    print(trunc(tele.read_time_to_ap(),1))
    time.sleep(1)
print("Activating engines for orbital insertion")
tele.throttle_full()
while tele.read_periapsis() < 80000:
    check_stage(tele)
    time.sleep(0.1)
tele.throttle_zero()
print("Pe reached " + str(tele.read_periapsis()/1000) + " km")
tele.rcs(0)


