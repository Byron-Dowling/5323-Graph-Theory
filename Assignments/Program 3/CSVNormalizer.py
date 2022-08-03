"""
    Name: Byron Dowling

    Parsing program I wrote to read a csv file and convert that data into
    a list of JSON objects or a JSON Array file
"""

import csv
import copy
import json

#create custom object of json object sample 

MyCustomObject = {
        "ID": 0,
        "City": " ",
        "State": " ",
        "Lat": " ",
        "Long": " "
}

#open file 
f = open('ufos.csv')

# read json
# data = json.load(f)
a = []
cities = []

#loop on json strings

count = 0

with open("ufos.csv") as csvfile:
    variable = csv.reader(csvfile, delimiter="\t")
    for row in variable:

        if len(cities) != 0:

            found = False

            for city in cities:

                if city == row[0]:

                    found = True
                    break

            if found == False:
                
                MyCustomObject["ID"] = count
                MyCustomObject["City"] = row[0]
                MyCustomObject["State"] = row[1]
                MyCustomObject["Lat"] = row[2]
                MyCustomObject["Long"] = row[3]

                cities.append(row[0])

                a.append(copy.deepcopy(MyCustomObject))

                count += 1

        else:

            MyCustomObject["ID"] = count
            MyCustomObject["City"] = row[0]
            MyCustomObject["State"] = row[1]
            MyCustomObject["Lat"] = row[2]
            MyCustomObject["Long"] = row[3]

            cities.append(row[0])

            a.append(copy.deepcopy(MyCustomObject))

            count += 1


print(a)

with open ('Texas_ufos.json', 'w') as fout:

    json.dump(a, fout, indent=4)


