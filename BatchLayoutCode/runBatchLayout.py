#!/usr/bin/python3
import sys
import os
import argparse
import random
import subprocess
import itertools
import pickle
import pprint
import numpy as np 
import matplotlib.pyplot as plt
from matplotlib import collections as mc
import warnings
import time
from scipy.io import mmread, mminfo
import networkx as nx
warnings.filterwarnings('ignore')

def runBatchLayout(inf, outd, iteration, batch, algo, init, initf, threads, bht, engt, w):
	if not os.path.exists('bin/BatchLayout'):
		print('Executible BatchLayout not found in bin directory!!!')
		sys.exit('Program Exiting...\n')
	command = "./bin/BatchLayout -input "+ str(inf) + " "
	if len(outd) > 0:
		command += "-output " + str(outd) + " "
	if iteration > 0:
		command += "-iter " + str(iteration) + " "
	if batch > 0:
		command += "-batch " + str(batch) + " "
	if algo >= 0:
		command += "-algo " + str(algo) + " "
	if init >=0 and init <= 1:
		if len(initf) > 0 :
			init = 2
			command += "-initf " + str(initf) + " "
		else:
			command += "-init " + str(init) + " "
	if threads > 0:
		command += "-threads " + str(threads) + " "
	if bht > 0:
		command += "-bht " + str(bht) + " "
	if engt > 0:
		command += "-engt " + str(engt) + " "
	if w > 0:
		command += "-w " + str(w) + " "
	print(command)
	try:
		start = time.time()
		subp = os.popen(command)
		output = subp.read()
		subp.close()
		end = time.time()
		#print("output:", output)	
		outputs = output.split("\n")
		print("Actual wall time for BatchLayout:", outputs[18].split(":")[1])
		outputdir = outputs[19].split(":")[1]
	except:
		print("Something went wrong in processing !!! Check error message and act accordingly.")
	return outputdir

def readCoordinates(filename):
	corfile = open(filename)
	X = []
	for line in corfile.readlines():
		l = line.split("\t")
		X.append([float(l[0]), float(l[1])])
	X = np.array(X)
	corfile.close()
	return X

def drawGraph(G, graph, X, algo1="Algo1"):
	gridsize = (1, 1)
	fig = plt.figure(figsize=(15, 10))
	axIN = plt.subplot2grid(gridsize, (0, 0))
	plt.axis('off')
	#axIN.set_title(algo1)
	axIN.set_xlim(min(X[:,0]), max(X[:,0]))
	axIN.set_ylim(min(X[:,1]), max(X[:,1]))
	linesIN = []
	e = 0
	for i,j in zip(*graph.nonzero()):
		if i>j:
			linesIN.append([X[i], X[j]])
			e += 1
	lcIN = mc.LineCollection(linesIN, linewidths=.3, color='blue')
	axIN.add_collection(lcIN)
	plt.axis('off')
	plt.savefig(algo1+'_drawing.png')
	 
if __name__ == '__main__':
	
	parser = argparse.ArgumentParser(description='BatchLayout tool usage.', add_help=True)
	
	parser.add_argument('-inf', '--inf', required=True, type=str, help='Input data file name with directory.')
	parser.add_argument('-outd', '--outd', required=False, default="", type=str, help='Output directory.')
	parser.add_argument('-iter', '--iter', required=False, default=500, type=int, help='Number of iteration.')
	parser.add_argument('-batch', '--batch', required=False, default=256, type=int, help='Size of batch.')
	parser.add_argument('-algo', '--algo', required=False, default=2, type=int, help='Algo option from 0 to 8.')
	parser.add_argument('-init', '--init', required=False, default=0, type=str, help='Initialization option between 0 and 1.')
	parser.add_argument('-initf', '--initf', required=False, default="", type=str, help='Initial coordinates.')	
	parser.add_argument('-threads', '--threads', required=False, default=0, type=int, help='Number of threads.')
	parser.add_argument('-bht', '--bht', required=False, default=0, type=float, help='Barnes-Hut threshold.')
	parser.add_argument('-engt', '--engt', required=False, default=0, type=float, help='Energy convergence threshold.')
	parser.add_argument('-w', '--w', required=False, default=0, type=float, help='Weight factor.')
	args = parser.parse_args()
	inputfile = args.inf
	outputfile = args.outd
	iteration = args.iter
	batch = args.batch
	algo = args.algo
	init = args.init
	initf = args.initf
	threads = args.threads
	barnesth = args.bht
	energyth = args.engt
	weight = args.w
	print("\n")
	print("@@@@@                   @      @                                     ")
	print("@    @                  @      @                                     ")
	print("@  @   @@@@@@ @  @@@@@@ @      @      @@@@@@ @    @ @@@@@@ @    @ @  ")
	print("@@@         @ @@ @    @ @@@@@@ @           @ @    @ @    @ @    @ @@ ")
	print("@  @   @@@@@@ @  @      @    @ @      @@@@@@ @@@@@@ @    @ @    @ @  ")
	print("@    @ @    @ @  @      @    @ @      @    @      @ @    @ @    @ @  ")
	print("@@@@@  @@@@@@ @@ @@@@@@ @    @ @@@@@@ @@@@@@ @@@@@@ @@@@@@ @@@@@@ @@ ")
	
	if os.path.isfile(inputfile):
		print("\n\nRunning Batchlayout...")
		outputdirector = runBatchLayout(inputfile, outputfile, iteration, batch, algo, init, initf, threads, barnesth, energyth, weight)
		print("Reading coordinates file...")
		X = readCoordinates(outputdirector)
		print("Drawing graph...")
		graph = mmread(inputfile)
		G = nx.Graph()
		splitted = inputfile.split("/")
		graphname = splitted[len(splitted)-1].split(".")[0]
		for i in range(mminfo(inputfile)[0]):
			G.add_node(i)
		for i,j in zip(*graph.nonzero()):
			if i > j:
				G.add_edge(i, j)
		drawGraph(G, graph, X, outputfile+""+graphname)
		print("Done!")
		print("Check following file:", outputfile+""+graphname+"_drawing.png")
		print("\n")
	else:
		print("Input file not found!!!\n")
