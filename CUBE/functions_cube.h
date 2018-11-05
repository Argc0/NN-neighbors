#ifndef __FUNCTIONS_CUBE__
#define __FUNCTIONS_CUBE__

#include "cube.h"

void get_args( int argc, char** argv, string& input_name, string& query_name, string& output_name, int& k, int& M, int &probes);

void hamming(int n, int i, int rest, vector<int>* num, int* counter, int max_probes);

void find_probes(int n, int probes, int d, vector<int> *num);

double NN_Search_cube(vector<TP> &q, int probes, int M, Bcube * cube, int metric, ofstream &myfile_out);

void Range_Search_cube(vector<TP> &q, double r, int c, Bcube * cube, int probes, int M, int metric, ofstream &myfile_out);

#endif