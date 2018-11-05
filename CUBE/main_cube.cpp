#include "functions_cube.h"


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

  clock_t begin_cube, begin_true, end_cube, end_true;
  double elapsed_secs_cube, elapsed_secs_true, sum_mean_time;

  double true_d, cube_d;
  vector<double> mean_approx;

  int k=0, M=0, probes=0;
  int n=-1 ,q=-1;

  Bcube * cube;
  //0 for euclidean, 1 for cosine
  int metric=0;
  //define
  int w = W;
  //read from query file
  double R = 0.0;

  string id;

  int i, dimension, g;

  srand((unsigned)time(0));

	get_args(argc, argv, input_name, query_name, output_name, k, M, probes);
	

	if(k == 0) k = 3;
  if(M == 0) M = 10;
  if(probes == 0) probes = 2;
  cout << "k: " << k << " M: " << M << " probes: "<< probes << endl;
   	
  if(input_name.empty()){
    cout << "Enter input name file: ";
    getline (cin,input_name);
  }
  cout << "The input name file is: " << input_name << endl;

  //read from input file
  myfile_in.open(input_name.c_str());
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

  //insert into cube analoga me ti metriki
 
  //k = log2(n);
  dimension = points[0][0].size();
  cube = new Bcube(k, dimension, w, metric);

  for (i = 0; i < n; i++){
    g = cube->insert_num(points[0][i]);
    cube->add_item(g, in_ids[0][i], g, &points[0][i]); 
  }

  if(output_name.empty()){
    cout << "Enter output name file: ";
    getline (cin,output_name);
  }
  cout << "The output name file is: " << output_name << endl;

  //strating query search
  while (1){
    if(query_name.empty()){
      cout << "Enter query name file: ";
      getline (cin,query_name);
    }
    cout << "The query name file is: " << query_name << endl;

    //reading query file

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

        if(R != 0)
          Range_Search_cube(query, R ,1 , cube, probes, M, metric, myfile_out);

        begin_cube = clock();
        cube_d = NN_Search_cube(query, probes, M, cube, metric, myfile_out);
        end_cube = clock();
        elapsed_secs_cube = double(end_cube - begin_cube) / CLOCKS_PER_SEC;

        begin_true = clock();
        true_d = true_distance(query, points, metric ,myfile_out);
        end_true = clock();
        elapsed_secs_true = double(end_true - begin_true) / CLOCKS_PER_SEC;

        //skip case that you dont find a neighbor
        if(cube_d != -1)
          mean_approx.push_back(cube_d/true_d);

        myfile_out << "tCube: " << elapsed_secs_cube << endl;
        myfile_out << "tTrue: " << elapsed_secs_true << endl;

        sum_mean_time += elapsed_secs_cube;

        query.clear();
        q++;
      }
      myfile_q.close();
    }else { 
      cout << "Unable to open query file and output file";
    }

    //ki mesos xronos evresis prosegistika kontinoterou gitona
    myfile_out << "Mean time of Hypercube search is: " << sum_mean_time/q << endl;
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
    +  sizeof(vector<string>) + n*sizeof(string)
    + n*sizeof(HashNode<TP>) + pow(2.0, double(k))*sizeof(HashBucket) + k*sizeof(map<int,int>) + sizeof(Bcube);

  if(metric){
    sum1 = k*dimension*sizeof(double) + k*sizeof(HCosine) + sizeof(GCosine);
  }else{
    sum1 = k*dimension*sizeof(double) + k*sizeof(HFunction) + k*sizeof(int) + sizeof(GFunction); 
  }

  cout << "The approximatly bytes are: " << sum + sum1 << endl;

  //free allocations
  in_ids->clear();
  delete in_ids;
  for(i=0; i< n; i++)
    points[0][i].clear();
  points->clear();
  delete points;

  delete cube;

  cout << "end\n";
  return 0;
}