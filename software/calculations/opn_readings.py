

def open_test_data(file_name: str):
    f = open(file_name, "r")
#print(f.readlines())
    no_tare_data = []
    tared_data = []
    tared = False

    for row in f:
        data = row.split()
        
        for i in data:
            if tared == False:
                no_tare_data.append(i)
            else:
                tared_data.append(i)
        tared = True

    


open_test_data("readings/05_1000.txt")