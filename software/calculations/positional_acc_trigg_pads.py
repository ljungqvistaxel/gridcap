import math
import os
import matplotlib.pyplot as pyplot

from opn_readings import open_test_data

def get_pos_acc():
    forces = [200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000]

    fig = pyplot.figure(figsize=(12,8))

    data_path = "readings/"

    pos_acc = {"05":0, "07":0, "10":0}

    for thickness in ["05", "07", "10"]:
        maxes = [] # maximum capacitance in reading
        for position in range(9):
            for force in forces:
                data_file_name = thickness + "_" + str(position) + "_" + str(force) + ".txt"

                if os.path.isfile(data_path + data_file_name):
                    #print("file found: " + data_file_name)

                    caps = open_test_data(data_path + data_file_name)

                    #force_ix = forces.index(force)
                    first = True
                    for pad_capacitance in caps:
                        if pad_capacitance >= 2.0 and first == True:
                            pos_acc.update({thickness:(pos_acc[thickness])+1})
                            first = False
                        elif pad_capacitance < 2.0:
                            pos_acc.update({thickness:(pos_acc[thickness])+1})


        #get average for one metal thickness:
        average_pos_acc = pos_acc[thickness] 
        average_pos_acc = (average_pos_acc/(64*9*10))*100
        #print(pos_acc[thickness] )
        print(thickness + ": " + str(average_pos_acc))
        

get_pos_acc()
