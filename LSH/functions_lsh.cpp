#include "functions_lsh.h"


//sinartisi pou epistrefei ta orismata apo tin grammi entolwn
void get_args( int argc, char** argv, string& input_name, string& query_name, string& output_name, int& k, int& L){
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
			} else if( !strcmp(argv[i], "-L") ) {
				L = atoi(argv[i+1]);
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


//anazitisi prosegistika kontinoterou geitona
double NN_Search(vector<TP> &q, HashTable ** table, GFunctions ** g, int L, int metric, ofstream &myfile_out){
	string key; 
	long long int num;
	double db = double(INT_MAX), dist;
	list<HashNode<TP>*> * temp;
	for(int i=0; i < L; i++){
		num = g[i]->getNum(q);
		temp = table[i]->return_bucket(g[i]->getBucket_Num(num));
		//if(temp->size() > 3*L) break;   //trick from algorithm
		for (std::list< HashNode<TP>* >::iterator it = temp->begin(); it != temp->end(); ++it){
			string k = (*it)->getKey();
			long long int g = (*it)->getG();
			if(g != num) continue;
			vector <TP> *p = (*it)->getValue();
			if(!metric)
				dist = euclidean_distance(q, *p);
			else
				dist = cosine_distance(q, *p);
			if( dist < db){
				db = dist;
				key = k;
			}	
		}
	}
	if(db == double(INT_MAX)){
		myfile_out << "Nearest neighbor: -" << endl;
		myfile_out << "distanceLSH: -" << endl;
		return -1;
	}
	myfile_out << "Nearest neighbor: " << key << endl;
	myfile_out << "distanceLSH: " << db << endl;

	return db;  
}

//anazitisi prosegistika geitonwn entos aktinas r
void Range_Search(vector<TP> &q, double r, int c, HashTable ** table, GFunctions ** g, int L, int metric, ofstream &myfile_out){
	//default c=1
	double dist;
	long long int num;
	//gia apofigi diplotipwn
	list<string> key_list;
	list<HashNode<TP>*> * temp;
	for(int i=0; i < L; i++){
		num = g[i]->getNum(q);
		temp = table[i]->return_bucket(g[i]->getBucket_Num(num));
		//if(temp->size() > 3*L) break; //trick from algorithm
		for (std::list< HashNode<TP>* >::iterator it = temp->begin(); it != temp->end(); ++it){
			string k = (*it)->getKey();
			long long int g = (*it)->getG();
			if(g != num) continue;
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
		}
	}
}