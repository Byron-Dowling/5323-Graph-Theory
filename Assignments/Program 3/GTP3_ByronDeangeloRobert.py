"""
        Name: 		Byron Dowling, Deangelo Brown, and Robert O'Brien
        Course:		5323 Graph Theory
        Semester:	Summer 2, 2022	
        Assignment:	Program 3 - Dijkstras Shortest Path

        Assignment Details:

            Write a program that will implement a shortest path algorithm. You may write your
            own code, if you are industrious, or use a pre-built algorithm.
            Use your program to run real data from any legitimate source. Data may be pulled
            directly into the program or you may copy/paste into a file. The type of data is
            entirely up to you; any data that can be mapped using a weighted graph is acceptable.
            Large data sets may be time consuming. It is acceptable to run on a portion of a data
            set. Document your code and cite your sources.

            Data source: https://cs.msutexas.edu/~griffin/data/
                - UFO Data
                - Condensed to only Texas
                - Further condensed for quicker run-time
"""

from cProfile import label
from random import randint
import networkx
import json
import matplotlib
import pydot
from math import radians, cos, sin, asin, sqrt


"""
        Idea of taking distance of GPS coordinates was ours, we orignally
        inserted Geeks for Geeks code directly into our code but they made
        a few mistakes so we tweaked it slightly to fit our needs.

        Code inspiration was borrowed from Geeks for Geeks
        https://www.geeksforgeeks.org/program-distance-two-points-earth
"""

def getdistance(lat1:float, lat2:float, lon1:float, lon2:float):
     
    """
            Need to convert the GPS coordinates from their degree values to
            their corresponding values in radians using the math modules'
            built-in radians function.
    """
    lon1 = radians(lon1)
    lon2 = radians(lon2)
    lat1 = radians(lat1)
    lat2 = radians(lat2)
      
    """
            Haversine formula: https://en.wikipedia.org/wiki/Haversine_formula

            Need to take the difference between the two pairs of lats and longs
            and then we need to take the Sine of the lat-difference squared and add
            that to the cosine of lattitude1 and then multiply by the cisone of 
            Lattitude2 and then multiply that by the Sine of the longitude difference
            squared.

            Next take the square root of this value and then plug that into the arcsine
            function and multiply the result by two for our C value. Finally, multiply
            C by the Radius of the earth and use the correct metric for if you are 
            wanting results in Kilometers or Miles.
    """

    dlon = (lon2 - lon1)
    dlat = (lat2 - lat1)

    a = (sin(dlat / 2)**2) + cos(lat1) * cos(lat2) * (sin(dlon / 2)**2)
 
    c = 2 * asin(sqrt(a))
    
    # Radius of earth in kilometers 6371. Use 3956 for miles
    r = 3956
      
    # calculate the result
    rt = c*r

    # Chop the decimal at two places
    rt = round(rt, 2)

    return(rt)


"""
 
 $$\      $$\           $$\                 $$$$$$$\  $$\                     $$\       
 $$$\    $$$ |          \__|                $$  __$$\ $$ |                    $$ |      
 $$$$\  $$$$ | $$$$$$\  $$\ $$$$$$$\        $$ |  $$ |$$ | $$$$$$\   $$$$$$$\ $$ |  $$\ 
 $$\$$\$$ $$ | \____$$\ $$ |$$  __$$\       $$$$$$$\ |$$ |$$  __$$\ $$  _____|$$ | $$  |
 $$ \$$$  $$ | $$$$$$$ |$$ |$$ |  $$ |      $$  __$$\ $$ |$$ /  $$ |$$ /      $$$$$$  / 
 $$ |\$  /$$ |$$  __$$ |$$ |$$ |  $$ |      $$ |  $$ |$$ |$$ |  $$ |$$ |      $$  _$$<  
 $$ | \_/ $$ |\$$$$$$$ |$$ |$$ |  $$ |      $$$$$$$  |$$ |\$$$$$$  |\$$$$$$$\ $$ | \$$\ 
 \__|     \__| \_______|\__|\__|  \__|      \_______/ \__| \______/  \_______|\__|  \__|
                                                                                        
                                                                                                                                                                         
"""
if __name__=='__main__':

    """
            We have two files that we have tested with, one is a ridiculously huge graph
            that has all UFO sightings in Texas and the condensed one is a hand picked
            and narrowed list of 14-15 cities.
    """
    f = open("Condensed_Texas_ufos.json")
    # f = open("Texas_ufos.json")

    ## After Specifying the file, load it as a JSON file
    data = json.load(f)

    ## New networkx MultiGraph declared
    G1 = networkx.Graph()


    """
            To ensure that we have a fully connected graph, we created 4 hubs that every
            other node will connect to at least one of these and the hubs will connect to
            each other. This will guarantee that we have a fully connected graph and not
            end up with isolated nodes when we randomize the connections.
    """
    Hubs = {
        "Austin": {"Lat": 30.2711286, "Long": -97.7436994},
        "Dallas": {"Lat": 32.7762719, "Long": -96.7968558},
        "Fort Worth": {"Lat": 32.753177, "Long": -97.3327458}, 
        "Houston": {"Lat": 29.675779, "Long": -95.40215851}
    }


    ## Creating nodes for each object in our JSON file
    for obj in data:
        G1.add_node(obj["ID"], ID=obj["ID"], City=obj["City"], Latitude=obj["Lat"], Longitude=obj["Long"])

        
    """
            Houston -> Austin -> Fort Worth -> Dallas

            Somehwat hard-coded values but this is to ensure that we will have a fully
            connected graph and not collections of sub graphs with isolated nodes.
    """
    G1.add_edge(G1.nodes[0]["ID"], G1.nodes[3]["ID"], label=162.3)     ## Houston -> Austin
    G1.add_edge(G1.nodes[3]["ID"], G1.nodes[2]["ID"], label=189.9)     ## Austin -> Fort Worth
    G1.add_edge(G1.nodes[2]["ID"], G1.nodes[1]["ID"], label=36.7)      ## Fort Worth -> Dallas


    """
            Looping through our Graph nodes and determining which of the hubs
            the city is most closest to and then creating an edge connection.

            For example, if the city is Stephenville TX, it would connect to
            the hub in Fort Worth as it is most closest to Fort Worth as compared
            to Dallas, Austin or Houston. 

            Likewise, Cedar Park TX would connect to the Austin hub, Sugar Land TX
            would connect to the Houston hub, Frisco TX would connect to the Dallas
            hub etc.

            To do this, we set distance to 10,000 an arbitrary value that would greatly
            exceed any normal value, then we check the distance of the city from the
            first hub and then check the other 3 hubs and updating the distance if 
            it is smaller. Once we have determined which hub is the closest, we make
            the edge connection with the label being the distance between them.
    """

    for i in range(len(G1.nodes)):

        distance = 10000.0
        cityhub = ""

        for key, value in Hubs.items():

            Lat1 = float(G1.nodes[i]["Latitude"])
            Long1 = float(G1.nodes[i]["Longitude"])
            
            temp = float() 
            temp = getdistance(Lat1, value["Lat"], Long1, value["Long"])

            if temp < distance:

                distance = temp
                cityhub = key
        
        for j in range(0,4,1):

            if cityhub == G1.nodes[j]["City"]:

                G1.add_edge(G1.nodes[i]["ID"], G1.nodes[j]["ID"], label=distance)



    """
            Another attribute that is somewhat hard-coded but was necessary to properly
            integrate the data into a full graph.

            We start looping through the nodes starting at 4 because we know indexes 0 
            through 3 are our 4 hubs and we don't randomized connections to them because
            they already have a significant amount.

            Code is written and some commented out to accomodate both datasets, but the 
            general idea is that we generate a random number within a reasonable range,
            1-3 for our condensed set and 12-24 for our larger set. This represents how
            many random connection edges we will make for each node. Then while looping
            through that number, we generate a random number between 4-13 or 4-85 to
            simulate the index number of the node in the JSON file, so if it generates
            the number 37, we will make an edge connection between the current one and
            the city that is at index 37, then we calculate the distance between them
            for the edge weight.
    """
    for i in range(4, len(G1.nodes), 1):

        x = randint(1,3)
        # x = randint(12,24)

        for j in range(x):
            y = randint(4, 13)
            # y = randint(4, 85)

            temp = getdistance(float(G1.nodes[i]["Latitude"]), float(G1.nodes[j]["Latitude"]),
            float(G1.nodes[i]["Longitude"]), float(G1.nodes[j]["Longitude"]))

            G1.add_edge(G1.nodes[i]["ID"], G1.nodes[j]["ID"], label=temp)


    ## Dictionary for the cities that is used in the graph presentation
    citylookup = {}

    ## Adding the cities to the dictionary.
    for i in range(len(G1.nodes)):

        city = G1.nodes[i]["City"]
        citylookup[i] = city

    """
            Running the single source Dijkstras shortest path where num is a randomly generated
            number to simulate a randomly selected node to start the Dijkstras shortest path
            algorithm from.

            The results are stored in a dictionary.
    """
    num = randint(0, len(G1.nodes))
    # length = dict(networkx.single_source_dijkstra_path_length(G1, G1.nodes[num]["ID"]))
    # length = networkx.single_source_dijkstra_path(G1, G1.nodes[num]["ID"])
    length = networkx.single_source_dijkstra(G1, G1.nodes[num]["ID"])

    print(type(length))
    print(length)

    """
        Plot the Graph

        We had trouble getting the edges to show properly since NetworkX does not seem to work
        well with multigraph multi-edges so we also made a dot file that can show in GraphViz
    """

    networkx.relabel_nodes(G1, citylookup, copy=False)

    pos = networkx.circular_layout(G1)

    G1.remove_edges_from(networkx.selfloop_edges(G1))
    mylabels = networkx.get_edge_attributes(G1, "label")
    networkx.draw(G1, pos, with_labels=True, node_size=3000)
    networkx.draw_networkx_edge_labels(G1, pos, mylabels)

    networkx.drawing.nx_pydot.write_dot(G1, 'multigraph.dot')

    # networkx.draw(G1)
    matplotlib.pyplot.show()


    
