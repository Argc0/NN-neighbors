#include "functions_cube.h"

//epistrefei ta orismata apo ti grammi entolwn
void get_args( int argc, char** argv, string& input_name, string& query_name, string& output_name, int& k, int& M, int &probes){
	int i, j;

	if (argc > 1) {

    	for (i = 1; i < argc-1; i += 2) {

			// Check if argument is given again
			for( j = i+2; j < argc-1; j += 2 ) {

				if( !strcmp(argv[i], argv[j]) ) {
					cout << "Error: Argument given again.\nPlease check README for more info.\n";
					exit(-1);
				}
			}
			// Get arguments
			if( !strcmp(argv[i], "-d") ) {
				input_name=argv[i+1];
			} else if( !strcmp(argv[i], "-q") ) {
				query_name = argv[i+1];
			} else if( !strcmp(argv[i], "-k") ) {
				k = atoi(argv[i+1]);
			} else if( !strcmp(argv[i], "-M") ) {
				M = atoi(argv[i+1]);
			} else if( !strcmp(argv[i], "-probes") ) {
				probes = atoi(argv[i+1]);
			} else if( !strcmp(argv[i], "-o") ) {
				output_name = argv[i+1];
			} else {
				cout << "Error: Wrong argument given.\nPlease check README for more info.\n";
				exit(-1);
			}
		}
	} else {
		cout << "The input, output, query files will be given by the user\n";
	}
	return;
}

//epistrefei olous tous geitones apo apostasi haming 1 mexri d
//https://stackoverflow.com/questions/40813022/generate-all-sequences-of-bits-within-hamming-distance-t
void hamming(int n, int i, int rest, vector<int>* num, int* counter, int max_probes) {
	if (rest == 0) {
		num->push_back(n);
		*counter = *counter + 1;
		return;
	}

	if (i < 0) return;

	if(*counter == max_probes) return;
	//change currebt bit
	n ^= 1U << i;
	hamming(n, i-1, rest-1, num, counter, max_probes);

	if(*counter == max_probes) return;
	//change it again(undo)
	n ^= 1U << i;
	hamming(n, i-1, rest, num, counter, max_probes);

}

//epistrefei sti domi num tous katallilous geitones apostasis hamming
void find_probes(int n, int probes, int d, vector<int> *num) {
	int *counter = new int;
	*counter=0;
	for (int i = 1 ; i <= d ; ++i) {
		hamming(n, d-1, i, num, counter, probes);
	}
	delete counter;
}

//evresi kontinoterou prosgistika geitona me ton cube
double NN_Search_cube(vector<TP> &q, int probes, int M, Bcube * cube, int metric, ofstream &myfile_out){
	string key; 
	int n;
	int d = cube->return_dimension();
	double db = double(INT_MAX), dist;
	int counter =0, examined_points = 0;
	list<HashNode<TP>*> * temp;

	vector<int> *num = new vector<int>();
	n = cube->insert_num(q);
	num->push_back(n);
	find_probes(n, probes-1, d, num);

	while( counter < probes){
		temp = cube->return_bucket(num[0][counter]);
		for (std::list< HashNode<TP>* >::iterator it = temp->begin(); it != temp->end(); ++it){
			string k = (*it)->getKey();
			vector <TP> *p = (*it)->getValue();
			if(!metric)
				dist = euclidean_distance(q, *p);
			else
				dist = cosine_distance(q, *p);
			if( dist < db){
				db = dist;
				key = k;
			}
			examined_points++;
			if(examined_points == M) break;	
		}
		if(examined_points == M) break;
		counter++;
	}
	num->clear();
	delete num;
	if(db == double(INT_MAX)){
		myfile_out << "Nearest neighbor: -" << endl;
		myfile_out << "distanceCube: -" << endl;
		return -1;
	}
	myfile_out << "Nearest neighbor: " << key << endl;
	myfile_out << "distanceCube: " << db << endl;
	
	return db;  
}

//evresi prosegistikwn geitonwn entos aktinas r me ton cube
void Range_Search_cube(vector<TP> &q, double r, int c, Bcube * cube, int probes, int M , int metric, ofstream &myfile_out){
	//default c=1
	double dist;
	int counter =0,n, examined_points =0;
	//gia apofigi diplotipwn
	list<string> key_list;
	list<HashNode<TP>*> * temp;

	int d = cube->return_dimension();

	vector<int> *num = new vector<int>();
	n = cube->insert_num(q);
	num->push_back(n);
	find_probes(n, probes-1, d, num);

	while( counter < probes){
		temp = cube->return_bucket(num[0][counter]);
		for (std::list< HashNode<TP>* >::iterator it = temp->begin(); it != temp->end(); ++it){
			string k = (*it)->getKey();
			vector <TP> *p = (*it)->getValue();
			if(!metric)
				dist = euclidean_distance(q, *p);
			else
				dist = cosine_distance(q, *p);
			if( dist < c*r){
				list<string>::iterator result1 = find(key_list.begin(), key_list.end(), k);
 
    			if (result1 == key_list.end()) {
        			key_list.push_back(k);
        			myfile_out << k << endl;
    			}
			}
			examined_points++;
			if(examined_points == M) break;	
		}
		if(examined_points == M) break;
		counter++;
	}
	num->clear();
	delete num;
}