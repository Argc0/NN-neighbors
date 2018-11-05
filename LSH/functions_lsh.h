#ifndef __FUNCTIONS_LSH__
#define __FUNCTIONS_LSH__


#include "hash.h"

void get_args( int argc, char** argv, string& input_name, string& query_name, string& output_name, int& k, int& L);

double NN_Search(vector<TP> &q, HashTable ** table, GFunctions ** g, int L, int metric, ofstream &myfile_out);

void Range_Search(vector<TP> &q, double r, int c, HashTable ** table, GFunctions ** g, int L, int metric, ofstream &myfile_out);

#endif