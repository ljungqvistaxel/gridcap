import os
import math
import matplotlib.pyplot as pyplot

from opn_readings import open_test_data
from capacitance_radius import get_capacitance_radius

def plot_capacitance_per_force_curves():

    forces = [200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000]

    fig = pyplot.figure(figsize=(12,8))

    data_path = "readings/"
    for thickness in ["05", "07", "10"]:
        maxes = [] # maximum capacitance in reading
        for position in range(9):
            sums = [] # sum of capacitances in reading
            radius_caps = [[], []] # capacitances and their radius from max point
            pos_maxes = [0.0]*10
            virtual_tare = 0
            for force in forces:
                data_file_name = thickness + "_" + str(position) + "_" + str(force) + ".txt"
                if os.path.isfile(data_path + data_file_name):
                    print("file found: " + data_file_name)

                    caps = open_test_data(data_path + data_file_name)

                    force_i = forces.index(force)

                    # virtual tare
                    if(force_i == 0):
                        virtual_tare = max(caps)

                    if(len(maxes) <= force_i):
                        maxes.append(0)
                    
                    maxes[force_i] += max(caps) - virtual_tare

                    pos_maxes[force_i] = max(caps) - virtual_tare

                    if(force == 2000):
                        print("cap@2000: " + str(max(caps)))

                    #radcap = get_capacitance_radius(caps)
                    #if(len(radcap) == 0):
                    #    continue
                    
                    #radius_caps[0].append(radcap[0])
                    #radius_caps[1].append(radcap[1])
                    
                    #pyplot.plot(radcap[0], radcap[1])

                    #maxes.append(max(caps))

            if(thickness == "05"):
                pyplot.plot(forces, pos_maxes, "b--", linewidth=0.5)
                pass
            elif(thickness == "07"):
                pyplot.plot(forces, pos_maxes, "r--", linewidth=0.5)
                pass
            elif(thickness == "10"):
                pyplot.plot(forces, pos_maxes, "g--", linewidth=0.5)
                pass

        if(len(maxes)==len(forces)):
            for i in range(len(forces)):
                maxes[i] = maxes[i] / 9

            if(thickness == "05"):
                pyplot.plot(forces, maxes, "b-", linewidth=2.0, label="0.5 mm")
                pass
            elif(thickness == "07"):
                pyplot.plot(forces, maxes, "r-", linewidth=2.0, label="0.7 mm")
                pass
            elif(thickness == "10"):
                pyplot.plot(forces, maxes, "g-", linewidth=2.0, label="1.0 mm")
                pass

            #pyplot.plot(forces, maxes, "k-", linewidth=2.0)
            pyplot.legend()
            pyplot.title("Capacitance response of different sheet metal thicknesses")
            pyplot.xlabel("applied force (mN)")
            pyplot.ylabel("capacitance reading (pF)")
            #pyplot.show()
            #pyplot.plot(forces, maxes, linewidth=2.0)
            
    pyplot.show()
