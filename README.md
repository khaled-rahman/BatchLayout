# SMU
Parallel Stochastic Minibatch Update (SMU) Algorithm for Force-Directed Graph Drawing in Shared Memory. This tool generates layout of graphs using multicores which is generally faster than equivalent algorithms. We assume that the input graph is undirected, connected and symmetric.

## System Requirements ##
Users need to have following softwares/tools installed in their linux based machine.
```
GCC version >= 4.9
OpenMP version >= 4.5
```
To use jupyter notebook for visualization, you need to have `apgl`,`scipy`,`networkx` python packages installed on your machine. You can install it using `pip` command as following:
```
pip install apgl scipy networkx
```

## Input ##
This version of SMU takes input graph in matrix market format. A sample dataset can be found in datasets directory.

## How to run ##
This tool is in executable format. So, you do not need to do any extra works. Just type the following:

```
./SMU datasets/input/3elt_dual.mtx 16 500
```
Here, datasets/input/3elt_dual.mtx is input graph, 16 is the number of threads and 500 is the iterations number. Note that this will report results for parallel version with minibatches 128 and 256 with corresponding running time. For maximum CPU utilization, please provide highest number of cores in your computing machine. In linux system, you can easily find it by typing `nproc` command in terminal. Output will be stored in datasets/output directory with corresponding name and minibatch size. This version reports coordinates of the graph which can be easily visualized using networkx. 

To run all experiments for all input datasets (assumed all settings are fine), type the following command:
```
bash runtools.sh
```

Users can see visualize.ipynb jupyter notebook. You can jupyter notebook using following command provided that you have it installed on your machine. Then set appropriate directory for input and output files.
```
jupyter notebook visualize.ipynb
```

## Output ##
Check datasets/output directory for output files. Output file has some naming convention for large set of experiments. `*SEQ*` represents sequential output. `*MINB*PARAOUTRAND*` represents output for SMU using provided number threads. Integer after `MINB` indicate size of minibatch and integer after `PARAOUTRAND` represents number of iterations used.

## Aesthetic Metrics ##
To compute aesthetic metrics (edge crossing, min-angle, edge length variation, shape gabriel), users will need to have [glam](https://github.com/VIDILabs/glam) tool installed which has some other system requirements. Users can also look at metrics folder to checkout more about it. Then type the following command (assumed all settings are fine) to compute metrics:

```
bash runmetrics.sh
```

To compute crosslessness or edge crossing, there is a program in metric folder which has dependency on [CGAL](https://www.cgal.org). To compile crosslessness.cpp, see instruction inside the file. To compute edge crossing type the following command:
```
./metrics/crosslessness datasets/input/3elt_dual.mtx < datasets/output/3elt_dual.mtxCACHEMINB128PARAOUTRAND600.txt
```
Here, 3elt_dual.mtx is the input graph file in matrix market format and 3elt_dual.mtxCACHEMINB128PARAOUTRAND600.txt is a text file which contains positions of vertices. To convert a .gml file to .txt file, see instructions in visualize.ipynb file.

If you have questions, please don't hesitate to ask me (Md. Khaledur Rahman) by sending email to morahma@iu.edu.
