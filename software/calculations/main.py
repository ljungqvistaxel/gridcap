import os
import matplotlib.pyplot as pyplot

from opn_readings import open_test_data

forces = [200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000]

data_path = "readings/"
for thickness in ["05", "07", "10"]:
    for position in range(9):
        maxes = []
        for force in forces:
            data_file_name = thickness + "_" + str(position) + "_" + str(force) + ".txt"
            if os.path.isfile(data_path + data_file_name):
                print("file found: " + data_file_name)

                caps = open_test_data(data_path + data_file_name)

                maxes.append(max(caps))

        if(len(forces) == len(maxes)):
            pyplot.plot(forces, maxes, linewidth=2.0)

    pyplot.title("thickness: " + thickness)
    pyplot.show()
