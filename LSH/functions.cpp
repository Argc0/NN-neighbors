#include "functions.h"

double internal_product(vector<TP> &p, vector<double> &v){
	int size = p.size();
	double sum = 0.0;
	for(int i = 0; i < size; ++i) {
		sum += p[i]*v[i];
	}

	return sum;
}

double euclidean_distance(vector<TP> &p, vector<TP> &v){
	int size = p.size();
	double d = 0.0;

	for(int i = 0; i < size; ++i) {
		//(y1-x1)^2
		d += (v[i] - p[i])*(v[i]-p[i]);
	}
	return sqrt(d);
}

double cosine_distance(vector<TP> &x, vector<TP> &y){
	int size = x.size();
	double d = 0.0, in = 0, a = 0, b = 0;

	//internal_product
	for(int i = 0; i < size; ++i) {
		in += x[i]*y[i];
	}

	for(int i = 0; i < size; ++i) {
		a += x[i]*x[i];
		b += y[i] *y[i]; 
	}
	d = in/(sqrt(a)*sqrt(b));
	return 1 - d;

}

//sinartisi pou epistrefei tin apostasi tou pragmatika kontinoterou geitona
double true_distance(vector<TP> &q, vector< vector<TP> > * points, int metric, ofstream &myfile_out){
	int n = points[0].size();
	double true_d = double(INT_MAX), d;
	if(metric){
		for(int x=0; x < n; x++){
            d = cosine_distance(q, points[0][x]);
            if( true_d > d){
            	true_d = d;
            }
        }
	}else{
		for(int x=0; x < n; x++){
            d = euclidean_distance(q, points[0][x]);
            if( true_d > d){
            	true_d = d;
            }
        }       
	}
	myfile_out << "distanceTrue: " << true_d << endl;
	return true_d;
}