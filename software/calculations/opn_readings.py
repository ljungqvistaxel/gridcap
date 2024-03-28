

def open_test_data(file_name: str):
    f = open(file_name, "r")

    no_tare_data = []
    tared_data = []
    tared = False
    ix = 0

    for row in f:
        data = row.split()
        
        for i in data:
            if tared == False:
                no_tare_data.append(float(i))
            else:
                tared_data.append(float(no_tare_data[ix]) - float(i))
                ix+=1
        tared = True #Second row has tared data

    print(tared_data)


open_test_data("readings/05_1000.txt")