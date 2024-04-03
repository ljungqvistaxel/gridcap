import os
import math
import matplotlib.pyplot as pyplot

from opn_readings import open_test_data
from capacitance_radius import get_capacitance_radius

forces = [200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000]

data_path = "readings/"
for thickness in ["05", "07", "10"]:
    for position in range(9):
        sums = [] # sum of capacitances in reading
        maxes = [] # maximum capacitance in reading
        radius_caps = [[], []] # capacitances and their radius from max point
        for force in forces:
            data_file_name = thickness + "_" + str(position) + "_" + str(force) + ".txt"
            if os.path.isfile(data_path + data_file_name):
                print("file found: " + data_file_name)

                caps = open_test_data(data_path + data_file_name)

                radcap = get_capacitance_radius(caps)
                if(len(radcap) == 0):
                    continue
                
                radius_caps[0].append(radcap[0])
                radius_caps[1].append(radcap[1])
                
                pyplot.plot(radcap[0], radcap[1])

                maxes.append(max(caps))

        if(len(forces) == len(maxes)):
            pyplot.title("thickness: " + thickness)
            pyplot.xlabel("radius from applied force (mm)")
            pyplot.ylabel("capacitance reading (mean)")
            pyplot.legend([str(f) for f in forces])
            pyplot.show()
            pass
            #pyplot.plot(forces, maxes, linewidth=2.0)
            
            

    
