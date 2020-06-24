# DynComm-R-package
***Dyn***amic Network ***Comm***unities Detection and Generation - R Package 


## Description
R package that implements several Algorithms used for evolving network analysis regarding community detection, using a common API.

Communities are calculated for the vertices and edges that change over time, without recalculating communities for the entire graph.

Algorithms use different, selectable, Criterion to evaluate the quality increase of the current community mapping.

Post-processing algorithms can be executed over the results to filter them for presentation without changing neither the graph nor the community mapping.

## Features

Currently implemented features are:

- Algorithms:

  - Dynamic Louvain Algorithm
  
- Criterion:

  - Classical Newman-Girvan, also called Modularity (Modularity)
  
- Post-processing algorithms:

  - Community Density Optimization

Features planned to be added:

- Algorithms:

  - TILES Community Detection [[2]](#2)

  - RDyn generation module for dynamic networks [[3]](#3)

- Criterion:

  - Balanced Modularity (BalMod)
  
- Post-processing algorithms:

  - Top N

  - Order by weight/edgeCount
  
- Implement stand alone User Interfaces

## Known bugs / limitations

### Not being fixed in the current version
- Edges that are not removed and calculations with differences in values: This is related to the way floating point numbers are subtracted in hardware and how numbers are represented in binary. When a number, like one third, that can not be completely represented in binary at a given number of bits (say 64 bit), is subtracted to another number, the result may deviate slightly from expected. When trying to hit exactly zero, like when removing edges, they may not be removed but linger with a very small value like 0.33X10^-17. The same happens when calculating weight aggregates, like the total weight of the graph, which influence the value of quality. These errors accumulate over time. A workaround is to snapshot the current graph and feed it to a new DynComm object. This is not being fixed in this version since it requires reimplementation of the graph object.


## Project directory structure

The following are the most important files/directories to be aware in the root of the package source code directory:

- **inst** contains a file named "REFERENCES.bib" that contains entries to referenced work.

- **man** contains the automatically generated help for DynComm available through the R help system.

- **R** is the source directory for R scripts.

- **src** is the source directory for other languages like C++ and Python.

- **tests** will contain, in the future, some automated tests to be performed when the package is built.

- **LICENSE** file describes the license agreement under which this project is shared.


## Software

To be able to use this package, the following software must be installed in your system, in the order presented. Search the web for more details on how to install and configure the software on your operating system, if required.

- R-4.0.0

- (optionally) RStudio 1.2.5042

## Installation

The latest version of this package (201901.01.0004) was tested in Xubuntu 20.04 64 bit and Windows 7 Ultimate 64 bit. 

You can install the stable version of this package from CRAN, if available, using the following command from within your R console:

`install.packages("DynComm")`

Otherwise, or to get the latest stable version, you need to compile the source code version provided in the root of the package source code directory.

### Additional steps for the source code version

Some additional software and configuration is required to compile and use the source code version of the package.

- On Windows operating systems:

  - rtools40
  - Add rtools to the path. Check [Rtools](https://cran.r-project.org/bin/windows/Rtools/) for details.

- On Linux operating systems:

  - a C++11 compatible compiler and development libraries

### Package Dependencies

The following packages are required to be installed from within R before trying to compile the source code version of the package. Some packages have dependencies of their own that may need fulfilling.

- Rcpp (>= 0.12.15)

- Rdpack

- roxygen2

- devtools


### Compiling the source code version

The safest route is to download the latest stable source code version file from the root of this project and call the following command from within your R console, replacing the "x" by the correct version number:

`install.packages("DynComm_xxxxxx.xx.xxxx.tar.gz", repos = NULL, type = "source")`

To get the most recent version, which may not be stable nor functional, you can use the source code of the GitHub master version using the following command from within your R console:

`devtools::install_github("softskillsgroup/DynComm-R-package/R-CRAN")`

If you get errors about paths, directories or files using Staged Installation (the default), you might have to change "StagedInstall: yes" to "StagedInstall: no" in the DESCRIPTION file in order to be able to compile and install locally.

## Documentation

Users please check either the **manual.pdf** file in the project repository or the R help system for the latest version of the manual. Beware that documentation in the R help system might be more up to date. R help can be called using one of the following commands from R console.

`help("DyComm-package")` or `??"DyComm-package"`

Developers please refer to the developer documentation in the R help system using one of the following commands from R console.

`help("DyComm-package-dev")` or `??"DyComm-package-dev"`


## Contributing

Please read the **CONTRIBUTING** file.

## License

This project is shared under GNU GPL version 2 or later. Please read the **LICENSE** file for more details.


## Referenced Work

This package uses the following work as reference material for the 
implementation of the algorithms.

- <a id="1">[1]</a>:
  - Title: Dynamic community detection in evolving networks using locality modularity optimization
  - Author: Cordeiro, Mário and Sarmento, Rui Portocarrero and Gama, João
  - Journal: Social Network Analysis and Mining
  - Date: 2016
  - Volume: 6
  - Number: 1
  - Pages: 1--20
  - Publisher: Springer Vienna

- <a id="2">[2]</a>:
  - Title: Tiles: An Online Algorithm for Community Discovery in Dynamic Social Networks
  - Author: Rossetti, Giulio and Pappalardo, Luca and Pedreschi, Dino and Giannotti, Fosca
  - Journal: Mach. Learn.
  - Date: August 2017
  - Volume: 106
  - Number: 8
  - ISSN: 0885-6125
  - Pages: 1213--1241
  - URL: https://doi.org/10.1007/s10994-016-5582-8
  - DOI: 10.1007/s10994-016-5582-8
  - ACMID: 3128003
  - Publisher: Kluwer Academic Publishers
  - Address: Hingham, MA, USA

- <a id="3">[3]</a>:
  - Title: RDYN: Graph Benchmark handling Community Dynamics
  - Author: Giulio Rossetti 
  - Journal: Journal of Complex Networks
  - Date: 2017-05-29
  - URL: https://academic.oup.com/comnet/article/5/6/893/3925036?guestAccessKey=c3470adf-391d-4fad-b935-63e71e4df06a
  - DOI: 10.1093/comnet/cnx016

- <a id="4">[4]</a>:
  - Title: Density-based Community Detection/Optimization
  - Author: Sarmento, Rui Portocarrero
  - Journal: arXiv
  - Date: Apr 2019
  - Eprint: 1904.12593
  - URL: https://arxiv.org/abs/1904.12593

