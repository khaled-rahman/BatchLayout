# SMU
Parallel Stochastic Minibatch Update (SMU) Algorithm for Force-Directed Graph Drawing in Shared Memory. This tool generates layout of graphs using multicores which is generally faster than equivalent algorithms. We assume that the input graph is undirected, connected and symmetric.

## System Requirements ##
Users need to have following softwares/tools installed in their PC.
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
./SMU datasets/input/3elt_dual.mtx 4 500
```
Here, datasets/input/3elt_dual.mtx is input graph, 4 is the number of threads and 500 is the iterations number. Note that this will report results for sequential version as well as parallel version. For maximum CPU utilization, please provide highest number of cores in your computing machine. In linux system, you can easily find it by typing `nproc` command in terminal. Output will be stored in datasets/output directory with corresponding name and minibatch size. This version reports coordinates of the graph which can be easily visualized using networkx. Users can see visualize.ipynb jupyter notebook. You can jupyter notebook using following command provided that you have it installed on your machine. Then set appropriate directory for input and output files.
```
jupyter notebook visualize.ipynb
```

## Output ##
Check datasets/output directory for output files. Output file has some naming convention for large set of experiments. `*SEQ*` represents sequential output. `*MINB*PARAOUTRAND*` represents output for SMU using provided number threads. Integer after `MINB` indicate size of minibatch and integer after `PARAOUTRAND` represents number of iterations used.


If you have questions, please don't hesitate to ask me (Md. Khaledur Rahman) by sending email to morahma@iu.edu.
