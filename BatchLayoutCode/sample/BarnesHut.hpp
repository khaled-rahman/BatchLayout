/*Some parts of this code has been followed from John J Dyer's implementation.
  * His github link is following: https://github.com/JohnJDyer
  * Restriction on this code is that no two vertices can have same coordinate.
 */

#ifndef _BARNESHUT_H_
#define _BARNESHUT_H_

#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <functional>
#include <fstream>
#include <iterator>
#include <ctime>
#include <map>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <array>
#include <stack>
#include <string>
#include <sstream>
#include <random>
#include <limits>
#include <cassert>
#include <parallel/algorithm>
#include <parallel/numeric>
#include <parallel/tags.h>
#include <ostream>
#include <tuple>

#include "../sample/commonutility.hpp"
#include "../sample/Coordinate.hpp"
#include "../sample/MortonCode.hpp"

using namespace std;

#define VALUETYPE double
#define INDEXTYPE int

struct Compare {
	Compare(int level, MortonCode *mcode): level(level), mcode(mcode) {}
	bool operator()(Coordinate<VALUETYPE> &a, int i ){
    		int bits = ((mcode->yscale(a.getY()) & level) > 0) * 2 + ((mcode->xscale(a.getX()) & level) > 0) * 1;
    		//cout << bits << ":" << i << "Comp" << level << ":" << mcode->yscale(a.getY()) << ":" << mcode->xscale(a.getX()) << endl;
		return bits < i;
  	}
  	int level;
  	MortonCode* mcode;
};

struct I_Node_2 : public Coordinate<VALUETYPE> {
	Coordinate<VALUETYPE> force = Coordinate<VALUETYPE>(0.0, 0.0);
	public:
	array<I_Node_2*, 4> _i_Coordinates;
        array<Coordinate<VALUETYPE> *, 4> _Coordinates;
        double diameter;  

	I_Node_2(array<I_Node_2*, 4> & i_Coordinates, array<Coordinate<VALUETYPE> *, 4>& Coordinates, VALUETYPE x, VALUETYPE y, VALUETYPE diameter):Coordinate<VALUETYPE>(x,y), _i_Coordinates(i_Coordinates), _Coordinates(Coordinates), diameter(diameter){
	}

  	~I_Node_2(){
    		for(auto i = 0; i < 4; i ++ ){
    			delete _i_Coordinates[i];
    		}
  	};

	void calc_resulsive_force(Coordinate<VALUETYPE> vertex, VALUETYPE theta, Coordinate<VALUETYPE> &force){
		Coordinate<VALUETYPE> temp = (Coordinate<VALUETYPE>) *this;
		VALUETYPE distance = (temp - vertex).getMagnitude();
		if(this->diameter / distance < theta){
			force = force + (temp - vertex) * (1.0 / distance);
		}else{
			for(int i = 0; i<4; i++){
				if(_i_Coordinates[i] != NULL){
					_i_Coordinates[i]->calc_resulsive_force(vertex, theta, force);
				}else if(_Coordinates[i] != NULL){
					Coordinate<VALUETYPE> t2 = (Coordinate<VALUETYPE>) *_Coordinates[i];
					VALUETYPE denominator = (t2 - vertex).getMagnitude();
					if(denominator > 0)
						force = force + (t2 - vertex) * (1.0 / denominator);
				}
			}
		}
	}

  	void print_tree(int n){	
		Coordinate<VALUETYPE> inode = (Coordinate<VALUETYPE>) *this;
  		for(int i = 0; i < n; i++)
    			cout << "\t";
  			cout << "i:x,y:" << inode.getX() << "\t"<< inode.getY() << "\n";
  			for(int i = 0; i < 4; i++){
    				if(_i_Coordinates[i] != NULL){
      					_i_Coordinates[i]->print_tree(n+1);
    				}else if(_Coordinates[i] != NULL) {
      					for(int a = 0; a < n+1; a++){
        					cout << "\t";
      					}
      					cout << "l:x,y:" << _Coordinates[i]->getX() << "\t"<< _Coordinates[i]->getY() << "\n";
    				}
  			}
	}
};

class BarnesHut{
	private:
	Coordinate<VALUETYPE> *nCoordinates;
	MortonCode mcode;
	INDEXTYPE V;
	VALUETYPE theta;
	I_Node_2* quadtree;
	public:
	BarnesHut(Coordinate<VALUETYPE> *vertices, INDEXTYPE N, VALUETYPE th): theta(th), mcode(vertices, N){
		nCoordinates = vertices;
		V = N;
		__gnu_parallel::_ThreadIndex t = 32;
		__gnu_parallel::parallel_tag pt(32);
		//pt.set_num_threads(t);
		//double s = omp_get_wtime(); 
		__gnu_parallel::sort(vertices, vertices + V, mcode, pt);
		//double e = omp_get_wtime();
		//printf("Sorting time:%lf\n", e - s);
		//for(int i = 0;i < V; i++)cout << i << ":"<< vertices[i].getX() << ":" << vertices[i].getY() << endl;
		#pragma omp parallel
		#pragma omp single
		{
			quadtree = build_tree(vertices, vertices + V, 1u << 31u, mcode.gridLength(1));
		}
	}
	BarnesHut() = delete;
	virtual ~BarnesHut(){
		delete quadtree;
	}
	I_Node_2* build_tree(Coordinate<VALUETYPE> *left,  Coordinate<VALUETYPE> *right, uint32_t level, VALUETYPE diameter){
		array<Coordinate<VALUETYPE> *, 5> bounds = {};
		array<Coordinate<VALUETYPE> *, 4> subgrids = {};
		array<I_Node_2*, 4> i_subgrids = {};
		bounds[0] = left;
		bounds[4] = right;
		
		auto comp = Compare(level, &mcode);
		#pragma omp parallel for
		for(INDEXTYPE i = 1; i < 4; i++){
			bounds[i] = lower_bound(left, right, i, comp);
		}
		/*cout << "level:" << level << endl;
		cout << "0:" << bounds[0]->getX() << ":" << bounds[0]->getY() << endl;
		cout << "1:" << bounds[1]->getX() << ":" << bounds[1]->getY() << endl;
		cout << "2:" << bounds[2]->getX() << ":" << bounds[2]->getY() << endl;
		cout << "3:" << bounds[3]->getX() << ":" << bounds[3]->getY() << endl;
		cout << "4:" << bounds[4]->getX() << ":" << bounds[4]->getY() << endl;
		cout << "............" << endl;
		*/
		I_Node_2 *c_0 = NULL, *c_1 = NULL, *c_2 = NULL, *c_3 = NULL;
		auto sub_level = level >> 1;
		auto sub_diameter = diameter / 2.0;
		
		
		int i = 0;
		const int make_child = 1;
		const int pthreshold = 45;
		if(bounds[i+1] - bounds[i] > make_child){
    			#pragma omp task shared(c_0) if(bounds[i+1] - bounds[i] > pthreshold)
    			c_0 = build_tree(bounds[i], bounds[i+1], sub_level, sub_diameter);
  		}
		i = 1;
  		if(bounds[i+1] - bounds[i] > make_child){
    			#pragma omp task shared(c_1) if(bounds[i+1] - bounds[i] > pthreshold)
    			c_1 = build_tree(bounds[i], bounds[i+1], sub_level, sub_diameter);
  		}
		i = 2;
		if(bounds[i+1] - bounds[i] > make_child){
    			#pragma omp task shared(c_2) if(bounds[i+1] - bounds[i] > pthreshold)
    			c_2 = build_tree(bounds[i], bounds[i+1], sub_level, sub_diameter);
  		}
		i = 3;
		if(bounds[i+1] - bounds[i] > make_child){
    			#pragma omp task shared(c_3) if(bounds[i+1] - bounds[i] > pthreshold)
    			c_3 = build_tree(bounds[i], bounds[i+1], sub_level, sub_diameter);
  		}
		
		for(int i = 0; i < 4; i++){
    			if(bounds[i+1] - bounds[i] == 1){
      				subgrids[i] = bounds[i];
    			}else{
      				subgrids[i] = NULL;
    			}
  		}
		
		#pragma omp taskwait
  		i_subgrids[0] = c_0;
  		i_subgrids[1] = c_1;
  		i_subgrids[2] = c_2;
  		i_subgrids[3] = c_3;

  		VALUETYPE c_x = 0.0, c_y = 0.0;
		
		VALUETYPE count = 0;
		
  		for(int i=0; i < 4; i ++){
    			if (i_subgrids[i] != NULL){
      				auto & sub = i_subgrids[i];
      				c_x += sub->getX();
      				c_y += sub->getY();
				count++;
    			}else if (subgrids[i] != NULL) {
      				auto & sub = subgrids[i];
      				c_x += sub->getX();
      				c_y += sub->getY();
				count++;
    			}
  		}
		if(count > 0){
  			c_x /= (count);
  			c_y /= (count);
		}
	
		return new I_Node_2(i_subgrids, subgrids, c_x, c_y, diameter);
	}
	Coordinate<VALUETYPE> calcRepForce(Coordinate<VALUETYPE> c){
		Coordinate<VALUETYPE> f = Coordinate<VALUETYPE>(0.0, 0.0);
		quadtree->calc_resulsive_force(c, theta, f) ;
		return f;
	}	
	void print_t(INDEXTYPE n){
		quadtree->print_tree(n);
	}
};

#endif
