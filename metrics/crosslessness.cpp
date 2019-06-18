//by Khaled
// compile: g++ crosslessness.cpp -o crosslessness -std=c++11  -lCGAL -lmpfr -lgmp -lpthread -static-libstdc++
// run ./crosslessness inp.mtx outcor.txt

#include<cmath>
#include<iostream>
#include<unordered_map>
#include<cstdio>
#include <list>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>
#include "CSR.h"

#define VALUETYPE double
#define INDEXTYPE int

typedef CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef Kernel::Point_2                                         Point_2;
typedef CGAL::Arr_segment_traits_2<Kernel>                      Traits_2;
typedef Traits_2::Curve_2                                       Segment_2;

double crosslessness(CSR<INDEXTYPE, VALUETYPE> &A_csr, vector<vector<double> > nCoordinates){
	vector<Segment_2> segments;
	double totalDeg = 0;
	double notPossible = 0;
	double allpossible = 0;
        for(int i =0; i< A_csr.rows; i++){
		double degi = A_csr.rowptr[i+1] - A_csr.rowptr[i];
		totalDeg += degi;
		notPossible += degi * (degi - 1);
        	for(int j = A_csr.rowptr[i]; j < A_csr.rowptr[i+1]; j++){
			if(i < A_csr.colids[j])
                        segments.push_back(Segment_2 (Point_2 (nCoordinates[i][0], nCoordinates[i][1]), Point_2 (nCoordinates[A_csr.colids[j]][0], nCoordinates[A_csr.colids[j]][1])));
                }
        }
	notPossible /= 2.0;
	totalDeg /= 2.0;
	allpossible = totalDeg * (totalDeg - 1) / 2;
        Segment_2 *segments2 = new Segment_2[segments.size()];
        segments2 = &segments[0];
        list<Point_2> pts;
        CGAL::compute_intersection_points(segments2, segments2 + segments.size(), std::back_inserter(pts));
	double maxcrossings = (allpossible - notPossible);
	double cll = maxcrossings > 0 ? (1.0 - 1.0 * pts.size() / maxcrossings):1.0;
	cout << "Allpossible:"<<allpossible << ", notpossible:"<<notPossible<<endl;
        cout << "Total " << pts.size() << " intersections and crosslessness:"<< cll << endl;
	return cll;
}

int main(int argc, char *argv[]){
	
	if(argc < 2)
    	{
        	cout << "Normal usage: ./crosslessness graphfile < coordinatefile" << endl;
        	return -1;
    	}
	string graphfile = argv[1];
	vector<vector<double> > coordinates;
	CSR<INDEXTYPE,VALUETYPE> A_csr(graphfile, false);
	double x, y;
	while(cin>>x>>y){
		vector<double> t;
		t.push_back(x);
		t.push_back(y);
		coordinates.push_back(t);
	}
	/*for(int i=0;i<coordinates.size(); i++){
		cout << "X=" << coordinates[i][0] << ", Y="<<coordinates[i][1] << endl;
	}*/
	double cll = crosslessness(A_csr, coordinates);
	cout << cll << endl;
	return 0;
}
