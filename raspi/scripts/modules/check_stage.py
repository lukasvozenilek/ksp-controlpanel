def check_stage(tele):
    try:
        global prev_lf,prev_sf
        
        sf = tele.read_resource('SolidFuel')
        if sf == None:
            return
        sf_speed = prev_sf - sf
        
        lf = tele.read_resource('LiquidFuel')
        if lf == None:
            return
        lf_speed = prev_lf - lf
    
        if (lf_speed <= 0 and lf != tele.read_resource_max('LiquidFuel')) or (sf_speed <= 0 and sf != tele.read_resource_max('SolidFuel')) and (tele.read_throttle() != 0):
            print("Fuel flow stopped - staging")
            tele.stage()
            time.sleep(5)
        prev_lf = lf
        prev_sf = sf
    except:
        print("Error")
