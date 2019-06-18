#ifndef _CSR_H_
#define _CSR_H_

#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <omp.h>

#include <random>

using namespace std;

template <class IT, class NT>
class CSR
{ 
public:
    CSR():nnz(0), rows(0), cols(0) {}
    CSR (string filename, bool transpose);
    void shuffleIds(); // Randomly permutating column indices
    void make_empty();
    void print();
    string filename;
    vector<IT> InDegree();
    vector<IT> OutDegree();
    ~CSR()
	{
        make_empty();
	}
    
	IT rows;	
	IT cols;
	IT nnz; // number of nonzeros
    
    IT *rowptr;
    IT *colids;
    NT *values;
};


// Read a graph stored in the matrix market format
// and store in the CSR format
template <class IT, class NT>
CSR<IT,NT>::CSR(const string filename, bool transpose)
{
    IT i;
    bool isUnsy;
    IT num, offset, tmp_nz;
    char *line, *ch;
    FILE *fp;
    IT *col_coo, *row_coo;
    NT *val_coo;
    IT *each_row_index;
    IT *nnz_num;
    const int LINE_LENGTH_MAX = 256; 
    this->filename = filename;
    isUnsy = false;
    line = (char *)malloc(sizeof(char) * LINE_LENGTH_MAX);
  
    /* Open File */
    fp = fopen(filename.c_str(), "r");
    if(fp == NULL)
    {
        cout << "File can not be opened....\n";
        exit(1);
    }
    do {
        fgets(line, LINE_LENGTH_MAX, fp);
        if (strstr(line, "general")) {
            isUnsy = true;
        }
    } while(line[0] == '%');
  
    /* Get size info */
    if(!transpose)
        sscanf(line, "%d %d %d", &rows, &cols, &tmp_nz);
    else
        sscanf(line, "%d %d %d", &cols, &rows, &tmp_nz);

    /* Store in COO format */
    num = 0;
    col_coo = (IT *)malloc(sizeof(IT) * (tmp_nz));
    row_coo = (IT *)malloc(sizeof(IT) * (tmp_nz));
    val_coo = (NT *)malloc(sizeof(NT) * (tmp_nz));

    while (fgets(line, LINE_LENGTH_MAX, fp)) {
        ch = line;
        /* Read first word (row id)*/
        if(!transpose)
            row_coo[num] = (IT)(atoi(ch) - 1);
        else
            col_coo[num] = (IT)(atoi(ch) - 1);
        ch = strchr(ch, ' ');
        ch++;
        /* Read second word (column id)*/
        if(!transpose)
            col_coo[num] = (IT)(atoi(ch) - 1);
        else
            row_coo[num] = (IT)(atoi(ch) - 1);
        ch = strchr(ch, ' ');

        if (ch != NULL) {
            ch++;
            /* Read third word (value data)*/
            val_coo[num] = (NT)atof(ch);
            ch = strchr(ch, ' ');
        }
        else {
            val_coo[num] = 1.0;
        }
        num++;
    }
    fclose(fp);

    /* Count the number of non-zero in each row */
    nnz_num = (IT *)malloc(sizeof(IT) * rows);
    for (i = 0; i < rows; i++) {
        nnz_num[i] = 0;
    }
    for (i = 0; i < num; i++) {
        nnz_num[row_coo[i]]++;
        if(col_coo[i] != row_coo[i] && isUnsy == false) {
            nnz_num[col_coo[i]]++;
            (tmp_nz)++;
        }
    }

    nnz = tmp_nz;

    /* Allocation of rpt, col, val */
    rowptr = (IT *)malloc(sizeof(IT) * (rows + 1));
    colids = (IT *)malloc(sizeof(IT) * (nnz));
    values = (NT *)malloc(sizeof(NT) * (nnz));

    offset = 0;
    for (i = 0; i < rows; i++) {
        rowptr[i] = offset;
        offset += nnz_num[i];
    }
    rowptr[rows] = offset;

    each_row_index = (IT *)malloc(sizeof(IT) * rows);
    for (i = 0; i < rows; i++) {
        each_row_index[i] = 0;
    }
  
    for (i = 0; i < num; i++) {
        colids[rowptr[row_coo[i]] + each_row_index[row_coo[i]]] = col_coo[i];
        values[rowptr[row_coo[i]] + each_row_index[row_coo[i]]++] = val_coo[i];
    
        if (col_coo[i] != row_coo[i] && isUnsy==false) {
            colids[rowptr[col_coo[i]] + each_row_index[col_coo[i]]] = row_coo[i];
            values[rowptr[col_coo[i]] + each_row_index[col_coo[i]]++] = val_coo[i];
        }
    }

    free(line);
    free(nnz_num);
    free(row_coo);
    free(col_coo);
    free(val_coo);
    free(each_row_index);
}





template <class IT, class NT>
vector<IT> CSR<IT,NT>::OutDegree()
{
    vector<IT> outDegree(rows,0);
#pragma omp parallel for
    for (IT i = 0; i < rows; ++i)
    {
        outDegree[i] =rowptr[i + 1] - rowptr[i];
    }
    return outDegree;
}

template <class IT, class NT>
vector<IT> CSR<IT,NT>::InDegree()
{
    vector<IT> inDegree(cols,0);
#pragma omp parallel for
    for (IT i = 0; i < rows; ++i)
    {
        for (IT j = rowptr[i]; j < rowptr[i + 1]; ++j)
        {
            IT col = colids[j];
            // synchonization is needed
            __sync_fetch_and_add(inDegree.data()+col, 1);
            //inDegree[col] ++;
        }
    }
    return inDegree;
}

// Randomly permute the matrix
template <class IT, class NT>
void CSR<IT,NT>::shuffleIds()
{
    mt19937_64 mt(0);
    for (IT i = 0; i < rows; ++i) {
        IT offset = rowptr[i];
        IT width = rowptr[i + 1] - rowptr[i];
        uniform_int_distribution<IT> rand_scale(0, width - 1);
        for (IT j = rowptr[i]; j < rowptr[i + 1]; ++j) {
            IT target = rand_scale(mt);
            IT tmpId = colids[offset + target];
            NT tmpVal = values[offset + target];
            colids[offset + target] = colids[j];
            values[offset + target] = values[j];
            colids[j] = tmpId;
            values[j] = tmpVal;
        }
    }
}

template <class IT, class NT>
void CSR<IT,NT>::make_empty()
{
    if( nnz > 0 ) {
        delete[] colids;
        delete[] values;
        nnz = 0;
    }
    if( cols > 0 ) {
        delete[] rowptr;
        cols = 0;
    }
    rows = 0;
}

template <class IT, class NT>
void CSR<IT,NT>::print(){
	stringstream  data(filename);
	filename = filename + "GB";
	ofstream output;
	output.open(filename);
	//cout << "Size of Row PTRS:" << A_csr.rows << ", Size of Col IDS:" << A_csr.nnz << endl;
	for(IT i = 0; i < rows; i++){
		output << i << " " << i << " " << 1 << endl;
		for(IT j = rowptr[i]; j < rowptr[i+1]; j++){
			output << colids[j] << " " << i << " " << values[j] << endl;
		}
	}
	output.close();
}


#endif
