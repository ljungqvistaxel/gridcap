import math
import os
import matplotlib.pyplot as pyplot

from opn_readings import open_test_data

def get_capacitance_radius(caps):
    pad_distance = 13.33 # mm

    max_i = 0;
    for i in range(64):
        if(caps[i] > caps[max_i]):
            max_i = i

    max_x = max_i % 8
    max_y = int(max_i / 8)
    max_c = caps[max_i]

    #print("max point: (" + str(max_x) + ", " + str(max_y) + ")")

    vals = [] # (radius, total capacitance, number of samples)
    for i in range(64):
        y = int(i / 8)
        x = i % 8
        r = math.sqrt(((x-max_x)*pad_distance)**2 + ((y-max_y)*pad_distance)**2)

        radius_found = False
        for j in range(len(vals)):
            if vals[j][0] == r:
                radius_found = True
                vals[j][1] += caps[i]
                vals[j][2] += 1
                break

        if(radius_found == False):
            vals.append([r, caps[i], 1])
        
    res_r = [] # radiuses
    res_c = [] # caps
    for i in range(len(vals)):
        r = vals[i][0]
        mean_cap = vals[i][1] / vals[i][2]
        res_r.append(r)
        res_c.append(mean_cap)

    sorted_res = sorted(zip(res_r, res_c))

    res_r = [i[0] for i in sorted_res]
    res_c = [i[1] for i in sorted_res]

    res = [res_r, res_c]

    #print(vals)
    #print("res: " + str(res))
    return res


def plot_radius_curves():

    forces = [200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000]

    #fig = pyplot.figure(figsize=(12,8))

    data_path = "readings/"
    for thickness in ["05", "07", "10"]:
        #fig = pyplot.figure(figsize=(12,8))
        force_rad = [forces, [0]*len(forces), [0]*len(forces)] # capacitances and their radius from max point
        for position in range(9):
            pos_plot = [forces,[0]*len(forces)]
            for force in forces:
                data_file_name = thickness + "_" + str(position) + "_" + str(force) + ".txt"
                if os.path.isfile(data_path + data_file_name):
                    #print("file found: " + data_file_name)

                    caps = open_test_data(data_path + data_file_name)
                    #force_i = forces.index(force)

                    radcap = get_capacitance_radius(caps)
                    # [[rad1, rad2, rad3], [cap1, cap2, cap3]]

                    radius = 0
                    for i in range(len(radcap[0])-1, 0, -1):
                        #print("i: " + str(i))
                        if(radcap[1][i] >= 1.0):
                            radius = radcap[0][i]
                            #print("radius: " + str(radius))
                            break

                    # add up for mean calculation
                    try:
                        fi = forces.index(force)
                        force_rad[1][fi] += radius
                        force_rad[2][fi] += 1

                        pos_plot[1][fi] = radius
                    except ValueError:
                        print("unknown index error")
                    
                    """if (position == 6):# and force == 2000):
                        print("force: " + str(force) + ", radius: " + str(radius))
                        if(thickness == "05"):
                            pyplot.plot(force, radius, "r-", linewidth=2.0)
                        if(thickness == "07"):
                            pyplot.plot(force, radius, "g-", linewidth=1.0)
                        if(thickness == "10"):
                            pyplot.plot(force, radius, "b-", linewidth=1.0)"""

                    """maxcap = max(radcap[1])
                    for i in range(len(radcap[1])):
                        #radcap[1][i] /= maxcap
                        if(radcap[1][i] < 0.0):
                            radcap[1][i] = 0.0

                    if (force == 600 and position == 5):
                        pyplot.plot(radcap[0], radcap[1])
                        for i in range(len(radcap[1])):
                            pyplot.plot(radcap[0][i], radcap[1][i], "bs")

                    print("Length radcap: [" + str(len(radcap[0])) + ", " + str(len(radcap[1])) + "]")
                    for i in range(len(radcap[0])):
                        print(str(i) + ": " + str(radcap[0][i]) + ", " + str(radcap[1][i]))"""
                    #if(len(radcap) == 0):
                    #    continue

                    

                    #radius_caps_temp = 
                    #radius_caps_temp.sort()

                    # divide for mean calculation

            #pyplot.plot(pos_plot[0], pos_plot[1], "k--", linewidth=0.5)

        for i in range(len(force_rad[2])):
            #print("index: " + str(i) + " / " + str(len(force_rad[2])-1))
            print(str(force_rad[1][i]) + " / " + str(force_rad[2][i]))
            force_rad[1][i] = (force_rad[1][i])/(force_rad[2][i])
                    
        pyplot.plot(force_rad[0], force_rad[1], label=thickness)

    pyplot.legend()
    pyplot.show()
            
if __name__ == '__main__':
    plot_radius_curves()