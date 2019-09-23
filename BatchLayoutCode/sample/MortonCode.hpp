#ifndef _MORTONCODE_H_
#define _MORTONCODE_H_

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
#include <stack>
#include <string>
#include <sstream>
#include <random>
#include <limits>
#include <cassert>
#include <parallel/algorithm>
#include <parallel/numeric>
#include <ostream>
#include <tuple>

#include "../sample/commonutility.hpp"
#include "../sample/Coordinate.hpp"

using namespace std;

#define VALUETYPE double
#define INDEXTYPE int


class MortonCode{
  	private:
	VALUETYPE  minX, minY, MAXMIN;
	INDEXTYPE V;
	Coordinate<VALUETYPE> *nCoordinates;
	public:
	
    	MortonCode(Coordinate<VALUETYPE> *vertices, INDEXTYPE N){
		this->nCoordinates = vertices;
		this->V = N;
		auto max_x = __gnu_parallel::max_element(nCoordinates, nCoordinates+N, MortonCode::compX);
		auto max_y = __gnu_parallel::max_element(nCoordinates, nCoordinates+N, MortonCode::compY);
		auto min_x = __gnu_parallel::min_element(nCoordinates, nCoordinates+N, MortonCode::compX);
		auto min_y = __gnu_parallel::min_element(nCoordinates, nCoordinates+N, MortonCode::compY);
		minX = min_x->getX();
		minY = min_y->getY();
		MAXMIN = max(max_y->getY() - min_y->getY(), max_x->getX() - min_x->getX());
	}
	static bool compX(Coordinate<VALUETYPE> &a, Coordinate<VALUETYPE> &b){
                return a.getX() < b.getX();
        }
        static bool compY(Coordinate<VALUETYPE> &a, Coordinate<VALUETYPE> &b){
                return a.getY() < b.getY();
        }
        inline bool less_msb(uint32_t x, uint32_t y) const{
                return (x < y) and (x < (x ^ y));
        }
    	uint32_t xscale(VALUETYPE x) const{
      		return (uint32_t)(0xffffffff * (x - minX) / MAXMIN);
	}
    	uint32_t yscale(VALUETYPE y) const{
      		return (uint32_t)(0xffffffff * (y - minY) / MAXMIN);
	}
    	double gridLength(INDEXTYPE level){
      		return MAXMIN / (1 << (level -1));
    	}
    	void printMortonCode(Coordinate<VALUETYPE> &b){
		uint32_t bx = xscale(b.getX());
  		uint32_t by = yscale(b.getY());

  		for(int i = 31; i >= 0; i--){
    			cout<<((by >> i) % 2);
    			cout<<((bx >> i) % 2) << ":";
  		}

  		cout<<endl;
	}

    	bool operator()(const Coordinate<VALUETYPE> &a, const Coordinate<VALUETYPE> &b) const{
		uint32_t _a[2] = {yscale(a.getY()), xscale(a.getX())};
  		uint32_t _b[2] = {yscale(b.getY()), xscale(b.getX())};
  		int j = 0;
  		uint32_t x = 0;

  		for (int k = 0; k < 2; k++) {
    			uint32_t y = _a[k] ^ _b[k];
    			if (less_msb(x, y)) {
      				j = k;
      				x = y;
    			}
  		}
  		return _a[j] < _b[j];
	}
};
#endif
