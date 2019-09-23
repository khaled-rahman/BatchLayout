# BatchLayout

Parallel Batch Update Algorithm for Force-Directed Graph Layout generation in Shared Memory. This tool generates layout of graphs using multicores which is generally faster than state-of-the-art algorithms. It comes with various features, e.g., it intializes layout using random technique as well as greedy technique (helps to converge faster), it provides several options like (a,r)-energy models and Edge weights, it can approximate repulsive forces by  Barnes-Hut approximation and greedy technique. We assume that the input graph is undirected, connected and symmetric.

## System Requirements

Users need to have following softwares/tools installed in their PC. The source code was compiled and run successfully in both linux and macOS.
```
GCC version >= 4.9
OpenMP version >= 4.5
Python3 packages: matplotlib, scipy, networkx, numpy
```
Some helpful links for installation can be found at [GCC](https://gcc.gnu.org/install/), [OpenMP](https://clang-omp.github.io) and [Environment Setup](http://heather.cs.ucdavis.edu/~matloff/158/ToolsInstructions.html#compile_openmp).

## Compile and Run BatchLayout

Go to `BatchLayoutCode` directory and follow the instructions step by step. Once you have compiled and generated a executible file, you can run BatchLayout from this directory as following:
```
$ ./BatchLayoutCode/bin/BatchLayout -input ./datasets/input/3elt_dual.mtx -output ./datasets/output/ -iter 600 -batch 256 -threads 32 -algo 2
```
For more options, check out readme file in `BatchLayoutCode` folder or type `./BatchLayoutCode/bin/BatchLayout` from this directory for help message.

## Run othertools

We have also made other tools (ForceAtlas2, OpenOrd, and tsNET) available here to run. Please go to `othertools` folder and follow instructions step by step.

## File conversion
Different tools takes different input format. To increase user friendly, We provide few python scripts for file conversion in `fileconversion` folder. User can check out corresponding readme file.

## Visualize layout
Users can check `visualize.ipynb` jupyter notebook for visualization. You can run jupyter notebook using following command provided that you have it installed on your machine. Then set appropriate directory for input and output files.
```
jupyter notebook visualize.ipynb
```

## Aesthetic Metrics
To compute aesthetic metrics (edge crossing, min-angle, edge length variation, shape gabriel), users will need to have [glam](https://github.com/VIDILabs/glam) tool installed which has some other system requirements. There is another repository by [Felice](https://github.com/felicedeluca/graphmetrics) for computing measures such as Edge Uniformity, Stress, Neighborhood Preservation, etc. Users can check that as well.

## Acknowledgement
Authors would like to thank Kwon, Felice and J Dyer for making their repositories public. Those were helpful for building script for other tools and running aesthetic metrics.


## Contact
If you have questions or comments, please don't hesitate to ask me (Md. Khaledur Rahman) by sending email to `morahma@iu.edu`.
