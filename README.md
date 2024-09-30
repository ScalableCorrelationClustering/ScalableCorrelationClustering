# SCC v1.0 [![Codacy Badge](https://app.codacy.com/project/badge/Grade/5a998310685742f588cca5b9002720b1)](https://app.codacy.com?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

The scalable corrleation clustering (SCC v1.0) framework.

We address the issue of graph clustering in signed graphs, which are characterized by positive and negative weighted edges representing attraction and repulsion among nodes, respectively. The primary objective is to efficiently partition the graph into clusters, ensuring that nodes within a cluster are closely linked by positive edges while minimizing negative edge connections between them. To tackle this challenge, we give a scalable multilevel algorithm based on label propagation and local search. Moreover, we give  a memetic algorithm that incorporates a multilevel strategy. This approach meticulously combines elements of evolutionary algorithms with local refinement techniques, aiming to explore the search space more effectively than repeated executions. 

Installation Notes
=====
## Downloading: 
You can download the proejct with the following command line:

```console
git clone https://github.com/ScalableCorrelationClustering/ScalableCorrelationClustering
```

## Compiling: 
Before you can start, you need to install the following software packages:

- you need OpenMPI (https://www.open-mpi.org/)

Once you installed the packages, just type 
```console
./compile_withcmake.sh 
```
In this case, all binaries, libraries and headers are in the folder ./deploy/ 

Note that this script detects the amount of available cores on your machine and uses all of them for the compilation process. If you don't want that, set the variable NCORES to the number of cores that you would like to use for compilation. 

Alternatively use the standard cmake build process:
```console 
mkdir build
cd build 
cmake ../ -DCMAKE_BUILD_TYPE=Release     
make 
cd ..
```
In this case, the binaries, libraries and headers are in the folder ./build

Graph Format
=====
The graph format used by our programs is the same as used by Metis, Chaco, and the graph format that has been used during the 10th DIMACS Implementation Challenge on Graph Clustering and Partitioning. The input graph has to be undirected, without self-loops and without parallel edges.

To give a description of the graph format, we follow the description of the Metis 4.0 user guide very closely. A graph $G=(V,E)$ with $n$ vertices and $m$ edges is stored in a plain text file that contains $n+1$ lines (excluding comment lines). The first line contains information about the size and the type of the graph, while the remaining $n$ lines contain information for each vertex of $G$. Any line that starts with `%` is a comment line and is skipped.

The first line in the file contains either two integers, `n` `m`, or (in our case) three integers, `n` `m` `f`. The first two integers are the number of vertices `n` and the number of undirected edges of the graph, respectively. Note that in determining the number of edges `m`, an edge between any pair of vertices `v` and `u` is counted *only once* and not twice, i.e., we do not count the edge `(v,u)` from `(u,v)` separately. The third integer `f` is used to specify whether or not the graph has weights associated with its vertices, its edges, or both. In our case, this should be set to 1 since we use edge weights. 

The remaining `n` lines of the file store information about the actual structure of the graph. In particular, the `i`th line (again excluding comment lines) contains information about the `i`th vertex. Depending on the value of `f`, the information stored in each line is somewhat different. In the most general form (when `f=11`, i.e., we have node and edge weights), each line has the following structure:

`c v_1 w_1 v_2 w_2 ... v_k w_k`

where `c` is the vertex weight associated with this vertex, `v_1, ..., v_k` are the vertices adjacent to this vertex, and `w_1, ..., w_k` are the weights of the edges. Note that the vertices are numbered starting from 1 (not from 0). Furthermore, the vertex-weights must be integers greater or equal to 0. However, they will be ignored by our programs.

   
Running Programs
=====
Running the multilevel algorithm is done using

```console
./deploy/signed_graph_clustering examples/soc-sign-epinions.graph --seed=0
```

Running the distributed memetic algortihm using 4 cores for 120 seconds is done using

```console
mpirun -n 4 ./deploy/signed_graph_clustering_evolutionary examples/soc-sign-epinions.graph --seed=0 --time_limit=120
```


Licence
=====
The program is licenced under MIT licence.
If you publish results using our algorithms, please acknowledge our work by quoting the following paper:

```
@misc{hausberger2024scalablemultilevelmemeticsigned,
      title={Scalable Multilevel and Memetic Signed Graph Clustering}, 
      author={Felix Hausberger and Marcelo Fonseca Faraj and Christian Schulz},
      year={2024},
      eprint={2208.13618},
      archivePrefix={arXiv},
      primaryClass={cs.DS},
      url={https://arxiv.org/abs/2208.13618}, 
}
```


