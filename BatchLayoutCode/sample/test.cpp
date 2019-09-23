#include <iostream>
#include <vector>
#include <algorithm>
#include <parallel/algorithm>
#include <cstdlib>
#include <ctime>
#include "omp.h"
#include "cstdlib"

#include "Coordinate.h"

#define VALUETYPE double
#define INDEXTYPE int

#define vsize 10000

using namespace std;

Coordinate<VALUETYPE> plusd(Coordinate<VALUETYPE> omp_in, Coordinate<VALUETYPE> omp_out){return omp_in + omp_out;}

#pragma omp declare reduction(plus:Coordinate<VALUETYPE>:omp_out += omp_in) initializer(omp_priv = Coordinate<VALUETYPE>(0.0, 0.0))

#pragma omp declare reduction(vplus:vector<Coordinate<VALUETYPE> > : \
                             transform(omp_out.begin(), omp_out.end(), omp_in.begin(), omp_out.begin(), plusd)) \
                    initializer(omp_priv = omp_orig)

int fib(int n)
{
	int i, j;
  	if (n<2)
    		return n;
  	else
    	{
       		#pragma omp task shared(i) firstprivate(n)
       		i=fib(n-1);

       		#pragma omp task shared(j) firstprivate(n)
       		j=fib(n-2);
       		
		#pragma omp taskwait
       		return i+j;
    	}
}


int main(int argc, char *argv[]){
	srand(unsigned(time(0)));
	int t = atoi(argv[1]);
	vector<int> A{0,3,5,7};
	vector<int> B{1,2,3,0,3,0,3,0,1,2};
	vector<int> index;
	int n = atoi(argv[2]);;
	/*for(int i=0; i<5; i++)index.push_back(i);
	for(int i = 0; i<5; i++){
		random_shuffle(index.begin(), index.end());
		for(int j=0; j<index.size(); j++){
			cout << index[j] << " ";
		}
		cout << endl;
	}
	int k = 0, j;
	int v = 9;
	printf("NUmber of threads = %d\n", omp_get_max_threads());
	#pragma omp parallel for num_threads(t)
	for(int i = 0; i < 100; i++){
		int v = 3;
		printf("v = %d, i = %d, threadid = %d\n", v, i, omp_get_thread_num());
		v++;
	}
	printf("v = %d\n", v);
	
	for(int i = 0; i<A.size(); i++){
		k = A[i];
		#pragma omp parallel for schedule(static) num_threads(t)
		for(j = 0; j < A.size(); j++){
			if(j == B[k]){
				printf("i(yes):%d # B[%d] = %d, thread = %d\n",i, k, B[k], omp_get_thread_num());
				if(k < B.size() && k < A[i+1]){
					k++;
				}
			}else{
				printf("i(no):%d # j = %d, k = %d, thread =  %d\n",i, j, k, omp_get_thread_num()); 
			}	
		}
	}*/
	/*for(int i = 0; i<A.size(); i++){
                for(j = 0; j < A.size(); j++){
			if(i == j){
				if(i < A.size() - 1){
					for(int k = A[i]; k < A[i+1]; k++)
						printf("Attraction: i = %d --- j = %d\n", i, B[k]);
				}else{
					for(int k = A[i]; k < B.size(); k++)
						printf("Attraction: i = %d --- j = %d\n", i, B[k]);
				}
			}else{
				printf("Repulstion: i = %d --- j = %d\n", i, j);
			}
                }
        }*/
	/*#pragma omp parallel for schedule(static) num_threads(t)
	for(int i = 0; i < 40; i += 4){
		for(int b = 0; b < 4; b++){
			printf("i = %d, b = %d, i + b = %d, thread = %d\n", i, b, i+b, omp_get_thread_num());
		}
	}*/
	/*	
	vector<Coordinate<VALUETYPE> > test1(5, Coordinate <VALUETYPE>(0.0, 0.0));
	vector<Coordinate<VALUETYPE> > test2(5, Coordinate <VALUETYPE>(5.0, 5.0));
	Coordinate<VALUETYPE> f = Coordinate <VALUETYPE>(0.0, 0.0);

	#pragma omp parallel for schedule(static) num_threads(t) reduction(plus:f)
	for(int i = 0; i < 5; i++){
		f += test2[i];
	}

	#pragma omp parallel for schedule(static) num_threads(t) reduction(vplus:test1)
	for(int i = 0; i < 5*vsize; i++){
                test1[i%5] += f;
        }
	
	printf("x = %lf, y = %lf\n", f.getX(), f.getY());
	for(int i = 0; i < 5; i++){
                printf("%d: x = %lf, y = %lf\n", i, test1[i].getX(), test1[i].getY());
        }*/
	int sum = 0;
	double start = omp_get_wtime();
	#pragma omp parallel shared(n) num_threads(t)
  	{
    		//#pragma omp single
    		//printf ("fib(%d) = %d\n", n, fib(n));
    		#pragma omp for schedule(static) reduction(+:sum)
		for(int i =0; i < n; i++)
		#pragma omp task
		{
			int a  = 2*i;
			sum += a * 2 + 5 - a * 2;
		}
  	}
	double end = omp_get_wtime();
	cout << "Time:" << end - start << endl;
	return 0;
}
