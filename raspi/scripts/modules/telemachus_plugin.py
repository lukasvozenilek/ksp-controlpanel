import json, urllib2, time, os, math, socket

url = ''

socket.setdefaulttimeout(1)


#Input definitions

def read_surfaceVelocity():
    fresh_json = json.load(urllib2.urlopen(url + 'v.surfaceVelocity'))
    result = fresh_json["alt"]
    return result

def read_surfaceVelocityx():
    fresh_json = json.load(urllib2.urlopen(url + 'v.surfaceVelocityx'))
    result = fresh_json["alt"]
    return result

def read_surfaceVelocityy():
    fresh_json = json.load(urllib2.urlopen(url + 'v.surfaceVelocityy'))
    result = fresh_json["alt"]
    return result

def read_surfaceVelocityz():
    fresh_json = json.load(urllib2.urlopen(url + 'v.surfaceVelocityz'))
    result = fresh_json["alt"]
    return result


def read_trueAnomaly():
    fresh_json = json.load(urllib2.urlopen(url + 'o.trueAnomaly'))
    result = fresh_json["alt"]
    return result
    
def read_angularvelocity():
    fresh_json = json.load(urllib2.urlopen(url + 'v.angularVelocity'))
    result = fresh_json["alt"]
    return result

def read_asl():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'v.altitude'))
        result = fresh_json[u'alt']
        return result
    except:
        print("Connection error")


def read_apoapsis():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'o.ApA'))
        result = fresh_json["alt"]
        return result
    except:
        print("Connection error")


def read_body():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'v.body'))
        result = fresh_json["alt"]
        return result
    except:
        print("Connection error")


def read_eccentricity():
    fresh_json = json.load(urllib2.urlopen(url + 'o.eccentricity'))
    result = fresh_json["alt"]
    return result


def read_facing(dimension):
    try:
        if dimension in ['pitch']:
            fresh_json = json.load(urllib2.urlopen(url + 'n.pitch'))
            result = int(fresh_json["alt"])
        elif dimension in ['yaw']:
            fresh_json = json.load(urllib2.urlopen(url + 'n.heading'))
            result = int(fresh_json["alt"])
        elif dimension in ['roll']:
            fresh_json = json.load(urllib2.urlopen(url + 'n.roll'))
            result = int(fresh_json["alt"])
        else:
            result = -1
        return result
    except:
        print("Connection error")


def read_heading():
    #Note: This returns facing:yaw, not your heading over land
    #Basically what the navball shows, not 'true' heading
    fresh_json = json.load(urllib2.urlopen(url + 'n.heading'))
    result = math.radians(int(fresh_json["alt"]))
    return result


def read_inclination():
    fresh_json = json.load(urllib2.urlopen(url + 'o.inclination'))
    result = math.radians(int(fresh_json["alt"]))
    return result


def read_missiontime():
    fresh_json = json.load(urllib2.urlopen(url + 'v.missionTime'))
    result = fresh_json["alt"]
    return result


def read_orbitalperiod():
    fresh_json = json.load(urllib2.urlopen(url + 'o.period'))
    result = fresh_json["alt"]
    return result


def read_orbitalvelocity():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'v.orbitalVelocity'))
        result = fresh_json["alt"]
        return result
    except:
        print("Connection error")


def read_periapsis():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'o.PeA'))
        result = fresh_json["alt"]
        if result == None:
            return 100
        return result
    except:
        print("Connection error")
        return 100


def read_resource(resource):
    try:
        reformated_resource = url + 'r.resource' + '[' + resource + ']'
        fresh_json = json.load(urllib2.urlopen(reformated_resource))
        result = fresh_json["alt"]
        return result
    except:
        print("Connection error")


def read_resource_max(resource):
    reformated_resource = url + 'r.resourceMax' + '[' + resource + ']'
    fresh_json = json.load(urllib2.urlopen(reformated_resource))
    result = fresh_json["alt"]
    return result


def read_surfacespeed():
    fresh_json = json.load(urllib2.urlopen(url + 'v.surfaceSpeed'))
    result = fresh_json["alt"]
    return result


def read_throttle():
    fresh_json = json.load(urllib2.urlopen(url + 'f.throttle'))
    result = fresh_json["alt"]
    return result

def read_time_to_ap():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'o.timeToAp'))
        result = fresh_json["alt"]
        if result == None:
            return 100
        else:
            return result
    except:
        print("Connection Error")
        return 100

def read_time_to_pe():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'o.timeToPe'))
        result = fresh_json["alt"]
        if result == None:
            return 2000
        return result
    except:
        print("Connection Error")
        return 2000


def read_universaltime():
    fresh_json = json.load(urllib2.urlopen(url + 't.universalTime'))
    result = fresh_json["alt"]
    return result


def read_verticalspeed():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'v.verticalSpeed'))
        result = fresh_json["alt"]
        return result
    except:
        print("Connection error")

def read_heightFromTerrain():
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'v.heightFromTerrain'))
        result = fresh_json["alt"]
        return result
    except:
        print("Connection error")


def read_longlat():
    fresh_json = json.load(urllib2.urlopen(url + 'v.long'))
    long = fresh_json["alt"]
    
    fresh_json = json.load(urllib2.urlopen(url + 'v.lat'))
    lat = fresh_json["alt"]
    
    return long,lat

def read_phaseAngle(body):
    try:
        fresh_json = json.load(urllib2.urlopen(url + 'b.o.phaseAngle['+str(body)+']'))
        PA = fresh_json["alt"]
        if PA == None:
            return 360
        return PA
    except:
        print("Connection error")
        return 360





# Output Definitions


def toggle_MapView():
    urllib2.urlopen(url + 'm.toggleMapView')


def MJ_off():
    try:
        urllib2.urlopen(url + 'mj.smartassoff')
    except:
        print("Connection error")

def MJ_pro():
    try:
        urllib2.urlopen(url + 'mj.prograde')
    except:
        print("Connection error")
def MJ_retro():
    try:
        urllib2.urlopen(url + 'mj.retrograde')
    except:
        print("Connection error")
def MJ_att2(h,p,r):
    try:
        urllib2.urlopen(url + 'mj.surface2[' + str(h) + ',' + str(p) + ',' + str(r) + ']')
    except:
        print("Connection error")
        
def MJ_att(h,p):
    try:
        urllib2.urlopen(url + 'mj.surface2[' + str(h) + ',' + str(p) + ']')
    except:
        print("Connection error")

def abort():
    urllib2.urlopen(url + 'f.abort')


def fly_by_wire(var):
    urllib2.urlopen(url + 'v.setFbW' + '[' + str(var) + ']')


def brake(var):
    if var == 2:
        fresh_json = json.load(urllib2.urlopen(url + 'v.brakeValue'))
        if fresh_json["alt"] == "True":
            return 1
        elif fresh_json["alt"] == "False":
            return 0
        else:
            return fresh_json["alt"]
    elif var == 1:
        urllib2.urlopen(url + 'f.brake' + '[' + 'true' + ']')
        #print 'Setting Brake to on'
    elif var == 0:
        urllib2.urlopen(url + 'f.brake' + '[' + 'false' + ']')
        #print 'Setting Brake to off'
    else:
        return (-1)
        #print 'Brake value was set wrong'


def gear(var):
    if var == 2:
        fresh_json = json.load(urllib2.urlopen(url + 'v.gearValue'))
        if fresh_json["alt"] == "True":
            return 1
        elif fresh_json["alt"] == "False":
            return 0
        else:
            return fresh_json["alt"]
        
    elif var == 1:
        urllib2.urlopen(url + 'f.gear' + '[' + 'true' + ']')
        #print 'Setting Gear to on'
    elif var == 0:
        urllib2.urlopen(url + 'f.gear' + '[' + 'false' + ']')
        #print 'Setting Gear to off'
    else:
        return (-1)
        #print 'Gear value was set wrong'


def light(var):
    if var == 2:
        fresh_json = json.load(urllib2.urlopen(url + 'v.lightValue'))
        if fresh_json["alt"] == "True":
            return 1
        elif fresh_json["alt"] == "False":
            return 0
        else:
            return fresh_json["alt"]
    elif var == 1:
        urllib2.urlopen(url + 'f.light' + '[' + 'true' + ']')
        #print 'Setting Light to TRUE'
    elif var == 0:
        urllib2.urlopen(url + 'f.light' + '[' + 'false' + ']')
        #print 'Setting Light to False'
    else:
        return (-1)
        #print 'Light value was set wrong'


def rcs(var):
    if var == 2:
        fresh_json = json.load(urllib2.urlopen(url + 'v.rcsValue'))
        if fresh_json["alt"] == "True":
            return 1
        elif fresh_json["alt"] == "False":
            return 0
        else:
            return fresh_json["alt"]
    elif var == 1:
        urllib2.urlopen(url + 'f.rcs' + '[' + 'true' + ']')
        #print 'Setting RCS to TRUE'
    elif var == 0:
        urllib2.urlopen(url + 'f.rcs' + '[' + 'false' + ']')
        #print 'Setting RCS to False'
    else:
        return (-1)
        #print 'RCS value was set wrong'


def sas(var):
    if var == 2:
        fresh_json = json.load(urllib2.urlopen(url + 'v.sasValue'))
        if fresh_json["alt"] == "True":
            return 1
        elif fresh_json["alt"] == "False":
            return 0
        else:
            return fresh_json["alt"]
    elif var == 1:
        urllib2.urlopen(url + 'f.sas' + '[' + 'true' + ']')
        #print 'Setting SAS to TRUE'
    elif var == 0:
        urllib2.urlopen(url + 'f.sas' + '[' + 'false' + ']')
        #print 'Setting SAS to False'
    else:
        return (-1)
        #print 'SAS value was set wrong'


def stage():
    try:
        urllib2.urlopen(url + 'f.stage')
    except:
        print("Connection error")


def set_facing(dimension, angle):
    #This is done by setting relative positions from 0 to 1, as a percent
    #This is based on the three bars in the lower left corner, NOT the Navball
    if dimension in ['pitch']:
        urllib2.urlopen(url + 'v.setPitch' + '[' + str(angle) + ']')

    elif dimension in ['yaw']:
        urllib2.urlopen(url + 'v.setYaw' + '[' + str(angle) + ']')

    elif dimension in ['roll']:
        urllib2.urlopen(url + 'v.setRoll' + '[' + str(angle) + ']')


def set_throttle(throttle):
    urllib2.urlopen(url + 'f.setThrottle' + '[' + str(throttle) + ']')


def ag(agn):
    urllib2.urlopen(url + 'f.ag' + agn)

def throttle_zero():
    try:
        urllib2.urlopen(url + 'f.throttleZero')
    except:
        print("Connection error")
    
def throttle_full():
    try:
        urllib2.urlopen(url + 'f.throttleFull')
    except:
        print("Connection error")

