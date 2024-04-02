import os

from opn_readings import open_test_data

path = "readings/"
for thickness in ["05", "07", "10"]:
    for position in range(9):
        for force in [200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000]:
            data_file_name = thickness + "_" + str(position) + "_" + str(force) + ".txt"
            if os.path.isfile(path + data_file_name):
                print("file found: " + data_file_name)

                caps = open_test_data(path + data_file_name)

                print("max: " + str(max(caps)))
                print("")
