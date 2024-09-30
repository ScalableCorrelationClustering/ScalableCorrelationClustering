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


Running Programs
=====

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


