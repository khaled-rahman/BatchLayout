import sys
import numpy as np
import time
import os
from scipy.io import mmread, mminfo
import networkx as nx 


if __name__ == "__main__":
    graphfile = sys.argv[1]
    graph = mmread(graphfile)
    G = nx.Graph()
    for i in range(int(mminfo(graphfile)[0])):
        G.add_node(i)
    for i,j in zip(*graph.nonzero()):
        if i > j:
            G.add_edge(i, j)
    deg = dict(G.degree()).values()
    #print G.degree().values()
    #print len(G)
    print("Average Degree:", sum(deg) / float(len(G)))
