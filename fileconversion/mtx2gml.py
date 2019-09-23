import numpy as np
import time
import os
import networkx as nx
import sys
from scipy.io import mmread, mminfo

if __name__ == "__main__":
	filename = sys.argv[1]
	graph = mmread(filename)
	G = nx.Graph()
	for i in range(mminfo(filename)[0]):
		G.add_node(i)
	for i,j in zip(*graph.nonzero()):
		if i > j:
			G.add_edge(i, j)
	nx.write_gml(G, filename+".gml")
