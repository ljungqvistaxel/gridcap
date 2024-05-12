import math
import os
import matplotlib.pyplot as pyplot

from opn_readings import open_test_data

def get_pos_acc():
    forces = [200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000]

    fig = pyplot.figure(figsize=(12,8))

    data_path = "readings/"

    module_pos_acc = {"05":0, "07":0, "10":0}
    pressure_point_ix = {"0":17, "1":19, "2":21, "3":33, "4":35, "5":37, "6":49, "7":51, "8":53}

    test_iterations = 0

    accies = []

    for thickness in ["05", "07", "10"]:
        average_cap_per_force_arr = [0] * len(forces)
        for position in range(9):
            for force in forces:
                data_file_name = thickness + "_" + str(position) + "_" + str(force) + ".txt"

                if os.path.isfile(data_path + data_file_name):
                    #print("file found: " + data_file_name)
                    test_iterations +=1

                    caps = open_test_data(data_path + data_file_name)
                    cap_sum = 0
                    tested_pad_cap = 0
                    for pad_ix in range(len(caps)):
                        pad_cap = caps[pad_ix]
                        if pad_ix == pressure_point_ix[str(position)] and pad_cap > 0:
                            tested_pad_cap = pad_cap
                            cap_sum += pad_cap
                        if pad_cap > 0 and pad_ix != pressure_point_ix[str(position)]:
                            cap_sum += pad_cap

                    ix_pos_acc = (tested_pad_cap/cap_sum)*100
                    average_cap_per_force_arr[forces.index(force)] += ix_pos_acc
                    module_pos_acc.update({thickness:(module_pos_acc[thickness]+ix_pos_acc)})

        for ix in range(len(forces)):
            average_cap_per_force_arr[ix] /= len(pressure_point_ix)

        if(thickness == "05"):
            pyplot.plot(forces, average_cap_per_force_arr, "b-", linewidth=2.0, label="0.5 mm")

        elif(thickness == "07"):
            pyplot.plot(forces, average_cap_per_force_arr, "r-", linewidth=2.0, label="0.7 mm")

        elif(thickness == "10"):
            pyplot.plot(forces, average_cap_per_force_arr, "g-", linewidth=2.0, label="1.0 mm")

        average_pos_acc = module_pos_acc[thickness]
        average_pos_acc = average_pos_acc/(test_iterations)

        print(thickness + " = " + str(average_pos_acc) + "%")

        test_iterations = 0

        accies.clear()

    pyplot.legend()
    pyplot.title("Positional accuracy of different sheet metal thicknesses")
    pyplot.xlabel("applied force (mN)")
    pyplot.ylabel("Accuracy (%)")
    pyplot.xticks(forces)
    pyplot.grid()
    
    pyplot.savefig('plots/positional_accuracy.png', bbox_inches='tight')
    pyplot.show()

get_pos_acc()
