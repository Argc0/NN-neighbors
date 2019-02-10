# NN-neighbors    
This program is used for building two structures to find nearsest neighbors in **_C++_**. The first structure **(1)** is implementing the **LSH algorithm** for vectors in the d-dimensional space based on the **_Euclidean metric_** and **_Cosine similarity_**. The second one **(2)** is implementing the random projection method at the **HyperCube**, which is based on **LSH** functions for **_Euclidean distance_** and for **_Cosine similarity_**.    

**How to run (using terminal):**  
**_LSH:_**  
./lsh -d \<input file> -q \<query file> -k \<int> -L \<int> -o \<output file>  
**_HyperCube:_**  
./cube -d \<input file> -q \<query file> -k \<int> -M \<int> -probes \<int> -o \<output file>  
