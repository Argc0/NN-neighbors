#ifndef __FUNCTIONS__
#define __FUNCTIONS__


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm> 
#include <ctime> 
#include <iterator>
#include <math.h> 
#include <random>


//a max number
#define INT_MAX 570089
//a max number for ri for lsh euclidean hash function (g)
#define MAX 20
//a w number for h functions of lsh euclidean
#define W 350
//a define variable that changes when the points are int or double
#define TP int

using namespace std;
using std::string;

double internal_product(vector<TP> &p, vector<double> &v);

double euclidean_distance(vector<TP> &p, vector<TP> &v);

double cosine_distance(vector<TP> &x, vector<TP> &y);

double true_distance(vector<TP> &q, vector< vector<TP> > * points, int metric, ofstream &myfile_out);

#endif
