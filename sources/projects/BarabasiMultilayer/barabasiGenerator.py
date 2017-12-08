import networkx as nx
import json
import sys

numberOfNodes = int(sys.argv[1])
filename = str(sys.argv[2])
m = 3

G = nx.barabasi_albert_graph(numberOfNodes, m)
data = nx.node_link_data(G)
with open(filename, 'w') as file:
    json.dump(data, file)
