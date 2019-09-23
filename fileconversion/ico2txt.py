import numpy as np
import sys
def readCoordinatesOO(filename, flag = False):
    corfile = open(filename)
    if flag:
        wfile = open(filename+".txt", "w")
    X = []
    for line in corfile.readlines():
        l = line.split("\t")
        X.append([float(l[1]), float(l[2])])
        if flag:
            wfile.write(str(float(l[1])) + "\t" + str(float(l[2])) + "\t" + str(int(l[0])) + "\n")
    X = np.array(X)
    corfile.close()
    if flag:
        wfile.close
    return X

if __name__ == "__main__":
    fname = sys.argv[1]
    readCoordinatesOO(fname, True)
