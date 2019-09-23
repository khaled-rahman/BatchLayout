#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <fstream>
#include <iterator>
#include <ctime>
#include <cstring>
#include <cmath>
#include <string>
#include <sstream>
#include <random>

#include "../sample/MortonCode.hpp"
#include "../sample/algorithms.hpp"
#include "../sample/BarnesHut.hpp"

using namespace std;

#define VALUETYPE double
#define INDEXTYPE int


void helpmessage(){
	printf("\n");
	printf("Usage of BatchLayout tool:\n");
	printf("-input <string>, full path of input file (required).\n");
	printf("-output <string>, directory where output file will be stored.\n");
	printf("-batch <int>, size of minibatch.\n");
	printf("-init <int>, any of 0 or 1, 1 - random initialization, 0 - greedy initialization.\n");
	printf("-initf <string>, this will overwrite -init and initialize coordinates from a given file.\n");
	printf("-iter <int>, number of iteration.\n");
	printf("-threads <int>, number of threads, default value is maximum available threads in the machine.\n");
	printf("-algo <int>, an integer among 0, 1, 2, 3, 4, 5 and 6.\n");
        printf("        0 - for sequential layout generation.\n");
        printf("        1 - for naive parallel approach for layout generation.\n");
        printf("        2 - for parallel layout generation using cache blocking stochastic minibatch update.\n");
        printf("        3 - for parallel layout generation using linlog mode, (0,-1)-energy model.\n");
        printf("        4 - for parallel layout generation using barnes-hut repulsive force approximation.\n");
        printf("        5 - for parallel layout generation using greedy force approximation approach.\n");
	printf("        6 - for parallel layout generation, 80%% time in -algo 4 and last 20%% time in -algo 2.\n");
	printf("        7 - for parallel layout generation, this will calculate forces using linear algebric format.\n");
	printf("        8 - for parallel layout generation, (1,-1)-energy model.\n");
	printf("-bht <float>, barnes-hut threshold parameter.\n");
	printf("-engt <float>, convergenece criteria. It should be a floating point number between 0 to 1 exclusive.\n");
        printf("It indicates if energy value is improved less than this percenrages then optimization will stop and return layout. If this value is set, then number of iteration will be overwritten.\n");
	printf("-weight <float>, a real number to include edge weight.\n");
	printf("-h, show help message.\n");

	printf("default: -weight 1.0 -bht 1.2 -engt 0.01 -batch 256 -iter 600 -threads MAX -algo 2 -init 0\n\n");
}


void TestAlgorithms(int argc, char *argv[]){
	VALUETYPE energyThreshold = 0.01, bhThreshold = 1.2, weight = 1.0;
	INDEXTYPE init = 0, batchsize = 256, iterations = 600, numberOfThreads = omp_get_max_threads(), algoOption = 2;
	string inputfile = "", initfile = "", outputfile = "", algoname = "CACHE", initname = "GREEDY";
	for(int p = 0; p < argc; p++){
		if(strcmp(argv[p], "-input") == 0){
			inputfile = argv[p+1];
		}
		if(strcmp(argv[p], "-output") == 0){
			outputfile = argv[p+1];
		}
		if(strcmp(argv[p], "-batch") == 0){
			batchsize = atoi(argv[p+1]);
		}
		if(strcmp(argv[p], "-init") == 0 && init != 2){
			init = atoi(argv[p+1]);
			if(init < 0 || init > 1) init = 1;
		}
		if(strcmp(argv[p], "-iter") == 0){
			iterations = atoi(argv[p+1]);
		}
		if(strcmp(argv[p], "-threads") == 0){
			numberOfThreads = atoi(argv[p+1]);
		}
		if(strcmp(argv[p], "-algo") == 0){
			algoOption = atoi(argv[p+1]);
		}
		if(strcmp(argv[p], "-bht") == 0){
			bhThreshold = atof(argv[p+1]);
		}
		if(strcmp(argv[p], "-initf") == 0){
			initfile = string(argv[p+1]);
			init = 2;
		}
		if(strcmp(argv[p], "-engt") == 0){
			energyThreshold = atof(argv[p+1]);
			if(energyThreshold < 0.0 || energyThreshold > 1.0) energyThreshold = 0.01;
		}
		if(strcmp(argv[p], "-weight") == 0){
			weight = atof(argv[p+1]);
		}
		if(strcmp(argv[p], "-h") == 0){
			helpmessage();
			exit(1);
		}
	}
	if(inputfile.size() < 1 || argc < 2){
		helpmessage();
		exit(1);
	}
	if(init == 1){
		initname = "RANDOM";
	}
	if(init == 2){
		initname = "FILE";
	}
	printf("\n");
        printf("@@@@@                   @      @                                     \n");
        printf("@    @                  @      @                                     \n");
        printf("@  @   @@@@@@ @  @@@@@@ @      @      @@@@@@ @    @ @@@@@@ @    @ @  \n");
        printf("@@@         @ @@ @    @ @@@@@@ @           @ @    @ @    @ @    @ @@ \n");
        printf("@  @   @@@@@@ @  @      @    @ @      @@@@@@ @@@@@@ @    @ @    @ @  \n");
        printf("@    @ @    @ @  @      @    @ @      @    @      @ @    @ @    @ @  \n");
        printf("@@@@@  @@@@@@ @@ @@@@@@ @    @ @@@@@@ @@@@@@ @@@@@@ @@@@@@ @@@@@@ @@ \n\n");
	CSR<INDEXTYPE, VALUETYPE> A_csr;
        SetInputMatricesAsCSR(A_csr, inputfile);
        A_csr.Sorted();
	if(algoOption < 0 || algoOption > 8) algoOption = 2;
	if(omp_get_max_threads() < numberOfThreads){
		numberOfThreads = omp_get_max_threads();
		if(A_csr.rows < 1000000 && numberOfThreads > 32){
			numberOfThreads = 32;
		}
	}
	vector<VALUETYPE> outputvec;
	algorithms algo = algorithms(A_csr, inputfile, outputfile, init, weight, energyThreshold, initfile);
	if(algoOption == 0){//sequential algo
		algoname = "SEQ";
                numberOfThreads = 1;
                batchsize = 1;
		outputvec = algo.seqAdjForceDirectedAlgorithm(iterations);
	}else if(algoOption == 1){//naive parallel algo
                algoname = "NAIVE";
		outputvec = algo.naiveParallelForceDirectedAlgorithm(iterations, numberOfThreads);
	}else if(algoOption == 2){//cache block minibatch algo
		algoname = "CACHE";
		if(fabs(energyThreshold - 0.01) > 0){
			outputvec = algo.cacheBlockingminiBatchForceDirectedAlgorithmConverged(iterations, numberOfThreads, batchsize);
		}else{
                	outputvec = algo.cacheBlockingminiBatchForceDirectedAlgorithm(iterations, numberOfThreads, batchsize);
		}
	}else if(algoOption == 3){//linlog mode
		algoname = "LINLOG";
                outputvec = algo.LinLogcacheBlockingminiBatchForceDirectedAlgorithm(iterations, numberOfThreads, batchsize);
	}else if(algoOption == 4){//barnes-hut approximation
		algoname = "BHCACHE";
		
                outputvec = algo.BarnesHutApproximation(iterations, numberOfThreads, batchsize, bhThreshold);
	}else if(algoOption == 5){//greedy approximation
		algoname = "GREEDY";
		outputvec = algo.approxForceDirectedAlgorithm(iterations, numberOfThreads, batchsize);
	}else if(algoOption == 6){//80% BH, 20% CB
		algoname = "BCGRDY";
		outputvec = algo.approxCacheBlockBH(iterations, numberOfThreads, batchsize);
	}else if(algoOption == 7){//O(n^+nd)
		algoname = "CACHESD";
		outputvec = algo.cacheBlockingminiBatchForceDirectedAlgorithmSD(iterations, numberOfThreads, batchsize);
	}else if(algoOption == 8){//(1,-1)-energy model
		algoname = "FA";
		outputvec = algo.FAcacheBlockingminiBatchForceDirectedAlgorithm(iterations, numberOfThreads, batchsize);
	}else if(algoOption == 9){
		algoname = "MINIB";
		outputvec = algo.miniBatchForceDirectedAlgorithm(iterations, numberOfThreads, batchsize);
	}
	string avgfile = "Results.txt";
        ofstream output;
       	output.open(avgfile, ofstream::app);
	output << algoname << "\t" << initname << "\t";
       	output << iterations << "\t" << numberOfThreads << "\t" << batchsize << "\t";
        output << outputvec[0] << "\t" << outputvec[1];
	output << endl;
	output.close();
}
int main(int argc, char* argv[]){
	
	TestAlgorithms(argc, argv);
        return 0;
}
