# BatchLayout

Parallel Batch Update Algorithm for Force-Directed Graph Layout generation in Shared Memory. This tool generates layout of graphs using multicores which is generally faster than equivalent algorithms. It comes with various features, e.g., it intializes layout using random technique as well as greedy technique (helps to converge faster), it provides several ptions like (a,r)-energy models and Edge weights, it can approximate repulsive forces by  Barnes-Hut approximation and greedy technique. We assume that the input graph is undirected, connected and symmetric.

```
@@@@@                   @      @                                     
@    @                  @      @                                     
@  @   @@@@@@ @  @@@@@@ @      @      @@@@@@ @    @ @@@@@@ @    @ @  
@@@         @ @@ @    @ @@@@@@ @           @ @    @ @    @ @    @ @@ 
@  @   @@@@@@ @  @      @    @ @      @@@@@@ @@@@@@ @    @ @    @ @  
@    @ @    @ @  @      @    @ @      @    @      @ @    @ @    @ @  
@@@@@  @@@@@@ @@ @@@@@@ @    @ @@@@@@ @@@@@@ @@@@@@ @@@@@@ @@@@@@ @@ 
```


## System Requirements

Users need to have following softwares/tools installed in their PC. The source code was compiled and run successfully in both linux and macOS.
```
GCC version >= 4.9
OpenMP version >= 4.5
Python3 packages: matplotlib, scipy, networkx, numpy
```
Some helpful links for installation can be found at [GCC](https://gcc.gnu.org/install/), [OpenMP](https://clang-omp.github.io) and [Environment Setup](http://heather.cs.ucdavis.edu/~matloff/158/ToolsInstructions.html#compile_openmp).

## Compile BatchLayout
To compile BatchLayout type the following command on terminal:
```
$ make clean
$ make
```
This will generate an executible file in bin folder.

## Run BatchLayout from command line

Input file must be in matrix market format ([check here for details about .mtx file](https://math.nist.gov/MatrixMarket/formats.html)). A lot of datasets can be found at [suitesparse website](https://sparse.tamu.edu). We provide few example input files in datasets/input directory. To run BatchLayout, use the following command:
```
$ ./bin/BatchLayout -input ./datasets/input/3elt_dual.mtx -output ./datasets/output/ -iter 600 -batch 256 -threads 32 -algo 2
```
Here, `-input` is the full path of input file, `-output` is the directory where output file will be saved, `-iter` is the number of iterations, `-batch` is the size of minibatch which is 256 here, `-threads` is the maximum number of threads which is 32 and `-algo` is the choice of algorithm to run which is 2 represending cache blocking stochastic minibatch update algorithm. All options are described below:
```
-input <string>, full path of input file (required).
-output <string>, directory where output file will be stored.
-batch <int>, size of minibatch.
-init <int>, any of 0 or 1, 1 - random initialization, 0 - greedy initialization.
-initf <string>, this will overwrite -init and initialize coordinates from a given file.
-iter <int>, number of iteration.
-threads <int>, number of threads, default value is maximum available threads in the machine.
-algo <int>, an integer among 0, 1, 2, 3, 4, 5 and 6.
        0 - for sequential layout generation.
        1 - for naive parallel approach for layout generation.
        2 - for parallel layout generation using cache blocking stochastic minibatch update.
        3 - for parallel layout generation using linlog mode, (0,-1)-energy model.
        4 - for parallel layout generation using barnes-hut repulsive force approximation.
        5 - for parallel layout generation using greedy force approximation approach.
        6 - for parallel layout generation, 80% time in -algo 4 and last 20% time in -algo 2.
        7 - for parallel layout generation, this will calculate forces using linear algebric format.
        8 - for parallel layout generation, (1,-1)-energy model.
-bht <float>, barnes-hut threshold parameter.
-engt <float>, convergenece criteria. It should be a floating point number between 0 to 1 exclusive.
It indicates if energy value is improved less than this percenrages then optimization will stop and return layout. If this value is set, then number of iteration will be overwritten.
-weight <float>, a real number to include edge weight.
-h, show help message.

default: -weight 1.0 -bht 1.2 -engt 0.01 -batch 256 -iter 600 -threads MAX -algo 2 -init 0
```

## Run BatchLayout using Python

User can run BatchLayout using python command. We have provided a python script which will run executible BatchLayout file, generate layout and draw graph based on the layout. Note that user must successfully compile BatchLayout to create executible file and then type following command:
```
$ python runBatchLayout.py -inf ./datasets/input/3elt_dual.mtx -outd ./datasets/output/ -iter 600 -threads 4
```
Type `python runBatchLayout.py -h` for help message. 

## Output File

Output layout will be stored as text file in provided output directory. If no directory is provided then it will be stored in working directory have a prefix of graph name in the filename. Each line of the output file represents two real numbers and one integer number which means following: `<x-coordinate> <y-coordinate> <nodeID>`. Output file will be generated by following a convension which is very useful for large-scale experiments.

```
SEQUADJ*<int> - will be generated by sequential algorithm where integer will represent number of iterations.

NAIVEPARA*<int> - will be generated by naive parallel algorithm where integer will represent number of iterations.

CACHEMINB*<int1>*PARAOUT*<int2> - will be generaged by cache blocking SMU where int1 is minibatch size and int2 is iterations.

LLCACHEMINB*<int1>*PARAOUT*<int2> - will be generaged by linlog mode of SMU where int1 is minibatch size and int2 is iterations.

BHMINB*<int1>*PARAOUT*<int2> - will be generaged by barnes-hut mode of SMU where int1 is minibatch size and int2 is iterations.

GAPPROX*<int1>*PARAOUT*<int2> - will be generaged by greedy approximation mode of SMU where int1 is minibatch size and int2 is iterations.
```
### Contact 
If you have questions, please don't hesitate to ask me (Md. Khaledur Rahman) by sending email to `morahma@iu.edu`.
