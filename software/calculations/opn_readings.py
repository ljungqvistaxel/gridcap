

def open_test_data(file_name: str):
    f = open(file_name, "r")

    no_tare_data = []
    tared_data = []
    is_offset = False
    ix = 0

    for row in f:
        data = row.split()
        for i in data:
            if is_offset == False:
                no_tare_data.append(float(i))
            else:
                tared_data.append(float(no_tare_data[ix]) - float(i))
                ix+=1
        is_offset = True #Second row has data to tare the first row

    print(tared_data)


open_test_data("readings/05_1000.txt")