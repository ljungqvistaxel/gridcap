import math

def get_capacitance_radius(caps):
    pad_distance = 10 # mm

    max_i = 0;
    for i in range(64):
        if(caps[i] > caps[max_i]):
            max_i = i

    max_x = max_i % 8
    max_y = int(max_i / 8)
    max_c = caps[max_i]

    print("max point: (" + str(max_x) + ", " + str(max_y) + ")")

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

    print(vals)
    print(res)
    return res
