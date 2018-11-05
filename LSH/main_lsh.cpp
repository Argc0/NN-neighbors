#include "functions_lsh.h"


int main(int argc, char *argv[]) {

  string line;
  istringstream iss;
  vector<TP> inputs;
  vector<TP> query;
  vector<string> * in_ids = new vector<string>();
  vector< vector<TP> > *points = new vector< vector<TP> >() ;

  string input_name, output_name, query_name;
  string answer;
  ifstream myfile_in, myfile_q;
  ofstream myfile_out;

  clock_t begin_lsh, begin_true, end_lsh, end_true;
  double elapsed_secs_lsh, elapsed_secs_true, sum_mean_time;

  double true_d, lsh_d=0;
  vector<double> mean_approx;

  int k=0, L=0;
  int n=-1 ,q=-1;

  //0 for euclidean, 1 for cosine
  int metric=0;
  //define
  int w = W;
  //read from query file
  double R = 0.0;

  string id;

  long long int g;

  int i, j, tablesz, dimension, in;

  srand((unsigned)time(0));

	get_args(argc, argv, input_name, query_name, output_name, k, L);
	

	if(k == 0) k = 4;
  if(L == 0) L = 5;
  cout << "k: " << k << " L: " << L << endl;
  	
  HashTable **table = new HashTable*[L]();
  GFunctions ** fs = new GFunctions*[L]();
  	
  if(input_name.empty()){
    cout << "Enter input name file: ";
    getline (cin,input_name);
  }
  cout << "The input name file is: " << input_name << endl;

  myfile_in.open(input_name.c_str());

  //read from input file the points
  if (myfile_in.is_open()){
    while ( getline (myfile_in,line) ){
      iss.clear();
      iss.str(line);


      if(n==-1){
        iss >> line;
        if( line.compare("euclidean") == 0){
          metric =0;
          n++;
          continue;
        }else if(line.compare("cosine") == 0){
          metric = 1;
          n++;
          continue;
        }else{
            n++;
        }
      }

      iss >> id;
      in_ids->push_back(id);

      copy((istream_iterator<TP>(iss)), istream_iterator<TP>(), back_inserter(inputs));

      points->push_back (inputs);
 
      inputs.clear();
      n++;
    }
    myfile_in.close();
  }else { 
    cout << "Unable to open input file";
  }
  
  //insert into hastable analoga me ti metriki
  dimension = points[0][0].size();
  if(metric){
    tablesz = pow(2.0, double(k));
    for(i=0; i < L; i++)
      fs[i] = new GCosine(k, dimension);

  }else{
    tablesz = n/4;
    for(i=0; i < L; i++)
      fs[i] = new GFunction(k, dimension, w, tablesz);   
  }

  for(i=0; i < L; i++){
    table[i] = new HashTable(tablesz);
    for (j = 0; j < n; j++){
      g = fs[i]->getNum(points[0][j]);
      in = fs[i]->getBucket_Num(g);
      
      table[i]->add_item(in, in_ids[0][j], g, &points[0][j]);
    }
  }

  if(output_name.empty()){
    cout << "Enter output name file: ";
    getline (cin,output_name);
  }
  cout << "The output name file is: " << output_name << endl;

  //starts query search
  while (1){
    if(query_name.empty()){
      cout << "Enter query name file: ";
      getline (cin,query_name);
    }
    cout << "The query name file is: " << query_name << endl;

    //read query input file
    myfile_q.open(query_name.c_str());
    myfile_out.open(output_name.c_str());
    
    sum_mean_time = 0.0;
    q=-1;
    if (myfile_q.is_open() && myfile_out.is_open()){
      while ( getline (myfile_q,line) ){
        iss.clear();
        iss.str(line);

        if(q==-1){
          iss >> line;
          long unsigned int position = line.find(":");
          //if first line empty continue
          if (position == std::string::npos){
            q++;
            R = 0;
          }else{
            string num = line.substr (position + 1);
            R =  stod(num);
            q++;
            continue;
          }
        }
        
        iss >> id;
        copy((istream_iterator<TP>(iss)), istream_iterator<TP>(), back_inserter(query));

        myfile_out << "Query: " << id << endl;
        myfile_out << "R-near neighbors:" << endl;

        if( R != 0)
          Range_Search(query, R, 1, table, fs, L, metric, myfile_out);
        
        begin_lsh = clock();
        lsh_d = NN_Search(query, table, fs, L, metric , myfile_out);
        end_lsh = clock();
        elapsed_secs_lsh = double(end_lsh - begin_lsh) / CLOCKS_PER_SEC;

        begin_true = clock();
        true_d = true_distance(query, points, metric ,myfile_out);
        end_true = clock();
        elapsed_secs_true = double(end_true - begin_true) / CLOCKS_PER_SEC;

        //skip case that you dont find a neighbor
        if(lsh_d != -1)
          mean_approx.push_back(lsh_d/true_d);

        myfile_out << "tLSH: " << elapsed_secs_lsh << endl;
        myfile_out << "tTrue: " << elapsed_secs_true << endl;

        sum_mean_time += elapsed_secs_lsh;

        query.clear();
        q++;
      }
      myfile_q.close();
    }else { 
      cout << "Unable to open query file and output file";
    }

    //ki mesos xronos evresis prosegistika kontinoterou gitona
    myfile_out << "Mean time of LSH search is: " << sum_mean_time/q << endl;
    myfile_out << "The max approximation is: " << *max_element(mean_approx.begin(),mean_approx.end()) << endl;

    myfile_out.close();

    cout << "Would you like to exit from the program? (yes or no)\n";
    getline(cin, answer);
    if(answer == "yes") break;
    query_name.clear();

  }

  //size of structures
  long int sum = 0, sum1 = 0;
  sum = (dimension*sizeof(TP))*n  + n*sizeof(vector<TP>) + sizeof(vector< vector<TP> >)
    +  sizeof(vector<string>) + n*sizeof(string) +
    + L*n*sizeof(HashNode<TP>) + L*tablesz*sizeof(HashBucket) + L*sizeof(HashTable);

  if(metric){
    sum1 = L*k*dimension*sizeof(double) + L*k*sizeof(HCosine) + L*sizeof(GCosine);
  }else{
    sum1 = L*k*dimension*sizeof(double) + L*k*sizeof(HFunction) + L*k*sizeof(int) +L*sizeof(GFunction); 
  }

  cout << "The approximatly bytes are: " << sum + sum1 << endl;

  //free allocations
  in_ids->clear();
  delete in_ids;
  for(i=0; i< n; i++)
    points[0][i].clear();
  points->clear();
  delete points;

  for(i=0; i<L; i++){
    delete fs[i];
  }

  delete[] fs;

  for(i=0; i<L; i++){
    delete table[i];
  }

  delete[] table;  

  cout << "end\n";
  return 0;
}