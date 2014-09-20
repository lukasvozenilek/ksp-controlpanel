from __future__ import division
from math import *
import time, os, subprocess, sys, serial, glob
import scripts.modules.telemachus_plugin as tele

try:
    ser = serial.Serial(port="/dev/ttyAMA0", baudrate=9600)
except:
    print("Error")
    exit()


tele.url = "http://192.168.0.12:8085/telemachus/datalink?alt="
update_ip = True
scripts = []
curpath = "/home/lukas/ksp-controlpanel/raspi/scripts/"

while (1):
    if update_ip:
        ip_file = open('ip_t.txt','r')
        ip = ip_file.read().rstrip()
        tele.url = (ip1 + ip + ip2)
        update_ip = False
        
    if update_scripts:
        scripts = glob.glob('/home/lukas/ksp-controlpanel/raspi/scripts/*.py')
        scripts2 = []
        for x in scripts:
            if "__init__" not in x:
                scripts2.append(x)
        scripts = scripts2
        
    try:
        if ser.inWaiting > 1:
            if ser.read(1) == '[':
                while ser.read()[len(ser.read())] != ']':
                    time.sleep(0.01)
                output = ser.read()
                
                if output[1] == 'C':
                    #Control
                    ag = output[2]
                    if ag == 1:
                        if tele.sas(2) == 1:
                            tele.sas(0)
                        elif tele.sas(2) == 0:
                            tele.sas(1)
                    elif ag == 2:
                        if tele.rcs(2) == 1:
                            tele.rcs(0)
                        elif tele.rcs(2) == 0:
                            tele.rcs(1)
                    elif ag == 3:
                        if tele.gear(2) == 1:
                            tele.gear(0)
                        elif tele.gear(2) == 0:
                            tele.gear(1)
                    elif ag == 4:
                        if tele.brake(2) == 1:
                            tele.brake(0)
                        elif tele.brake(2) == 0:
                            tele.brake(1)
                    elif ag == 5:
                        if tele.light(2) == 1:
                            tele.light(0)
                        elif tele.light(2) == 0:
                            tele.light(1)
                    elif ag == 6:
                        tele.ag(11)
                    elif ag == 7:
                        tele.abort()
                    elif ag == 8:
                        tele.toggleMapView()
                    elif ag > 11:
                        tele.ag(ag)
                        
                        
                elif output[1] == 'T':
                    #
                    count = 0
                    x = "".join(output)
                    x = x.replace("[","")
                    x = x.replace("]","")
                    x = x.replace("T","")
                    try:
                        throttle = float(x)
                        tele.set_throttle(throttle)
                    except:
                        print("Incorect format")
                        
                        
                elif output[1] == 'I':
                    x = "".join(output)
                    x = x.replace("[","")
                    x = x.replace("]","")
                    x = x.replace("T","")
                    if output[2] == 1:
                        ip_t_file = open('ip_t.txt','w+')
                        ip_t_file.write(x)
                        ip_t_file.close()
                    elif outputp[2] == 2:
                        ip_m_file = open('ip_m.txt','w+')
                        ip_m_file.write(x)
                        ip_m_file.close()
                    update_ip = True
                    
                elif output[1] == 'M':
                    pass
                
                elif output[1] == 'H':
                    os.system('sudo halt')
                    exit()
                
                elif output[1] == 'R':
                    if output[2] == 1:
                        scripts = glob.glob('/home/lukas/ksp-controlpanel/raspi/scripts/*.py')
                        scripts2 = []
                        for x in scripts:
                            if "__init__" not in x:
                                scripts2.append(x)
                        scripts = scripts2
                        #Send packet with script data
                    elif output[2] == 2:
                        #Send LED status
                        ser.write(tele.sas(2))
                        ser.write(tele.rcs(2))
                        ser.write(tele.gear(2))
                        ser.write(tele.brake(2))
                        ser.write(tele.light(2))
                    
                elif output[1] == 'A':
                    if output[2] == 1:
                        tele.MJ_pro()
                    elif output[2] == 2:
                        tele.MJ_retro()
                    elif output[2] == 3:
                        x = "".join(output)
                        x = x.replace("[","")
                        x = x.replace("]","")
                        x = x.replace("A","")
                        x = x.replace("3","")
                        x = x.split(',')
                        tele.mj_att(x[0],x[1],x[2])
                    elif out[2] == 4:
                        tele.MJ_off()
                        
                elif output[1] == 'S':
                    path = scripts[output[2]]
                    proc = subprocess.Popen([sys.executable, path])
                    while True:
                        if (ser.read(1) == "E"):
                            #kill process
                            proc.kill()
                            break
                        time.sleep(0.01)
                
            ser.flushInput()
    except:
        print("Error")
    time.sleep(0.01)