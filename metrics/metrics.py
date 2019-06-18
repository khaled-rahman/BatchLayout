import numpy as np
import time
import os
import argparse
from scipy.io import mmread, mminfo
import networkx as nx

def genGraphviz(g, x, filename):
    print("This function requires glam software...\n glam can be found in https://github.com/VIDILabs/glam ...\n")
    filename = filename+".dot"
    print ('Metrics for graph:', filename)
    output = open(filename, "w")
    output.write("graph G {\n")
    for i in range(len(x)):
        output.write(str(i) + " [x=\"" + str(x[i][0]) + "\", y=\"" + str(x[i][1]) + "\"];\n")
    visited = []
    e = 0
    for i,j in zip(*g.nonzero()):
        if i < j:
            t = i
            i = j
            j = t
        if i != j and [i,j] not in visited:
            output.write(str(i) + "--" + str(j) +";\n")
            e += 1
            visited.append([i, j])
    output.write("}")
    output.close()
    print("Total edges for glam:", e)
    command_crosslessness = "glam " + filename + " -m crosslessness" 
    command_edge_length = "glam " + filename + " -m edge_length_cv" 
    command_minangle = "glam " + filename + " -m min_angle" 
    command_shapedel = "glam " + filename + " -m shape_delaunay" 
    command_shapegab = "glam " + filename + " -m shape_gabriel" 
    metric_crosslessness = (os.popen(command_crosslessness).readlines()[2]).strip()
    metric_edgelength = (os.popen(command_edge_length).readlines()[2]).strip()
    metric_minangle = (os.popen(command_minangle).readlines()[2]).strip()
    metric_sdelaunay = (os.popen(command_shapedel).readlines()[2]).strip()
    metric_sgabriel = (os.popen(command_shapegab).readlines()[2]).strip()
    print(filename.split("/")[len(filename.split("/"))-1], metric_crosslessness, metric_edgelength, metric_minangle, metric_sdelaunay, metric_sgabriel)

def main(filename, lfile):
    graph = mmread(filename)
    G = nx.Graph()
    for i in range(int(mminfo(filename)[0])):
        G.add_node(i)
    for i,j in zip(*graph.nonzero()):
        if i > j:
            G.add_edge(i, j)
    X = []
    Xfile = open(lfile)
    for line in Xfile.readlines():
        l = line.split("\t")
        X.append([float(l[0]), float(l[1])])
    X = np.array(X)
    genGraphviz(graph, X, lfile)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Graph drawing metrics calculation pipeline...', add_help=True)
    parser.add_argument('-f', '--f', required=True, type=str, help='Filename of graph in mtx format.')
    parser.add_argument('-l', '--l', required=True, type=str, help='Layout file.')
    args = parser.parse_args()
    ffile = args.f
    lfile = args.l
    if os.path.isfile(ffile) and os.path.isfile(lfile):
        main(ffile, lfile)
    else:
        print("File not found!")
