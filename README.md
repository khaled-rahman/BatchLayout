# BatchLayout

Parallel Batch Update Algorithm for Force-Directed Graph Layout generation in Shared Memory. This tool generates layout of graphs using multicores which is generally faster than equivalent algorithms. It comes with various features, e.g., it intializes layout using random technique as well as greedy technique (helps to converge faster), it provides options like LinLog mode and Edge weights, it can approximate repulsive forces by  Barnes-Hut approximation and greedy technique. We assume that the input graph is undirected, connected and symmetric.

## System Requirements

Users need to have following softwares/tools installed in their PC.
```
GCC version >= 4.9
OpenMP version >= 4.5
```

## Compile and Run

Input file must be in matrix market format ([check here for details about .mtx file](https://math.nist.gov/MatrixMarket/formats.html)). To check compile and run, use the following command:
```
$ ./BatchLayout -input ./datasets/input/3elt_dual.mtx -output ./datasets/output/ -iter 600 -batch 256 -threads 32 -algo 2
```
Here, `-input` is the full path of input file, `-output` is the directory where output file will be saved, `-iter` is the number of iterations, `-batch` is the size of minibatch which is 256 here, `-threads` is the maximum number of threads which is 32 and `-algo` is the choice of algorithm to run which is 2 represending cache blocking stochastic minibatch update algorithm. All options are described below:

```
-input <string>, full path of input file.
-output <string>, directory where output file will be stored.
-batch <int>, size of minibatch.
-init <int>, either 0 or 1, 0 - random initialization, 1 - greedy initialization.
-iter <int>, number of iteration.
-threads <int>, number of threads, default value is maximum available threads in the machine.
-algo <int>, an integer among 0, 1, 2, 3, 4 and 5.
                0 - for sequential layout generation.
                1 - for naive parallel approach for layout generation.
                2 - for parallel layout generation using cache blocking stochastic minibatch update.
                3 - for parallel layout generation using linlog mode.
                4 - for parallel layout generation using barnes-hut repulsive force approximation.
                5 - for parallel layout generation using greedy force approximation approach.
-bht <float>, barnes-hut threshold parameter.
-engt <float>, convergenece criteria. It should be a floating point number between 0 to 1 exclusive.
                It indicates if energy value is improved less that this percenrages then optimization will stop and return layout.
-weight <float>, a real number to include edge weight.
-h, show help message.

default: -weight 1.0 -bht 1.2 -engt 0.01 -batch 256 -iter 600 -threads MAX -algo 2 -init 1
```

## Output File
Output layout will be stored as text file in provided output directory. If no directory is provided then it will be stored in working directory with `output` prefix in filename. Each line of the output file represents two real numbers and one integer number which means following: `<x-coordinate> <y-coordinate> <nodeID>`. Output file will be generated by following a convension which is very useful for large-scale experiments.

```
SEQUADJ*<int> - will be generated by sequential algorithm where integer will represent number of iterations.

NAIVEPARA*<int> - will be generated by naive parallel algorithm where integer will represent number of iterations.

CACHEMINB*<int1>*PARAOUT*<int2> - will be generaged by cache blocking SMU where int1 is minibatch size and int2 is iterations.

LLCACHEMINB*<int1>*PARAOUT*<int2> - will be generaged by linlog mode of SMU where int1 is minibatch size and int2 is iterations.

BHMINB*<int1>*PARAOUT*<int2> - will be generaged by barnes-hut mode of SMU where int1 is minibatch size and int2 is iterations.

GAPPROX*<int1>*PARAOUT*<int2> - will be generaged by greedy approximation mode of SMU where int1 is minibatch size and int2 is iterations.
```

## Visualize layout
Users can check `visualize.ipynb` jupyter notebook for visualization. You can run jupyter notebook using following command provided that you have it installed on your machine. Then set appropriate directory for input and output files.
```
jupyter notebook visualize.ipynb
```

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

If you have questions, please don't hesitate to ask me (Md. Khaledur Rahman) by sending email to `morahma@iu.edu`.
