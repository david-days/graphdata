# graphData

## Overview

### The Problem

Graph structures are used to represent many conceptual and real-world phenomena.  Throughout history, they have been used to solve optimization problems in domains from military strategy to language processing, and the use of graphs and combinatorial algorithms is the basis for much of our current world's technology.

With all of this utility, "optimization" of the graph data structures themselves presents a problem.  Code and libraries that provide fast algorithms to solve graph problems almost always use an internal and/or specialized graph structure implementation in that solution.  Purpose-built graph structures can greatly speed up algorithms, but come at the cost of losing general usefulness.

This leaves follow-on graph developers with a harsh choice:  Use an existing graph implementation and adjust their own work to match "the blessed way", or roll their own.  While not all graph structures are difficult or complex, spending time figuring out the side effects of choosing between a dense array structure, sparse linked-list (or double-linked) adjacencies, or any other core implementation can eat up a significant amount of valuable development time.  Testing and validation become their own beasts.  And if that graph data needs to be shared or stored and retrieved, then the amount of code balloons rapidly before one line of an actual solution algorithm can be written.

### A Solution

`graphData` is a project whose entire purpose is to provide a variety of standard, useful graph structure implementations and provide a general, common, and _unopinionated_ API for building, manipulating, and reading the state of those graphs.

The goal is simply to allow algorithm and solution developers to concentrate on representing and solving their particular problem, rather than worrying about basic structures and common API methods.

The structures and functions in `graphData` are self-contained implementations.  They are intended to be open-ended in their use, i.e. there is no "2D graph", "3D graph", "n-dimensional graph"; graph initialization options are used to tune the type and structure of the graph to be built.

## Available Backends

This version of `graphData` has several graph structures available.  Below is a table listing the available data structure types and a very general explanation of the best uses of that structure:

| Backend Type | Underlying Structure | Generally Best for... |
| --- | --- | --- |
| `LINKED` | Doubly linked nodes, with an edge list on each node | Small or modest sparse graphs with arbitrary ids, when the size is not known ahead of time |
| `HASHED` | Hash table of those node chains; grows as nodes are added | Larger sparse graphs, when nodes are looked up by id |
| `ARRAY` | Fixed index `0 .. size-1`, degree limited by the dimension count | Grids and other dense graphs whose size and maximum degree are known up front |
| `SHARED_MEM` | The array layout, stored in shared memory | A fixed graph that another process on the same machine will attach to |
| `SHARED_MMAP` | The array layout, stored in mapped files | A fixed graph that should remain on disk and be mapped again |

## Initialization

`initGraph` takes a flag combination, a label count, and an optional dimensions pointer. Omit a group and that group uses its default. Passing no flags yields `UNDIRECTED | LINKED | GENERIC | UNLABELED`.

| Group | Options | Default | Notes |
| --- | --- | --- | --- |
| Direction | `UNDIRECTED`, `DIRECTED` | `UNDIRECTED` | Directed edges are stored as given. An undirected edge is one link |
| Backend | the five above | `LINKED` | Choose one. Naming two backends is rejected |
| Domain | `GENERIC`, `SPATIAL` | `GENERIC` | Stored on the graph. This version does not change the layout from it |
| Labels | `UNLABELED`, `LABELED` | `UNLABELED` | `LABELED` requires a label count greater than zero |
| Access | omit, or `RD_ONLY` | writable | `RD_ONLY` is accepted only on `SHARED_MEM` and `SHARED_MMAP` |

`ARRAY`, `SHARED_MEM`, and `SHARED_MMAP` require a dimensions pointer. `LINKED` and `HASHED` take `NULL`. On the array-layout backends, `LABELED` also multiplies the node count by the label count.

## Building

The library is C, built with CMake 3.25 or newer. Compiler warnings are errors (`-Wall -Werror`). The Linux build lives in `graphDataLib`.

```sh
cd graphDataLib
./buildLinux.sh        # configure and build
./buildLinux.sh -t     # build and run the test suite
./buildLinux.sh -p     # also build the RPM and the binary tar.gz
./buildLinux.sh -i     # install the library that was just built
./buildLinux.sh -d     # also build the optional Doxygen output
./buildLinux.sh -h
```

On Linux the install prefix is `/usr`. The install includes `libgraphData.so`, the public headers, and `lib/pkgconfig/graphdata.pc`. `./buildLinux.sh -p` writes the RPM and the binary tar.gz under `output/`. The source tarball is a separate step: `make -C build-linux-x64 package_source`.

## Usage

Installed programs include `graphData.h`, `graphInit.h`, `graphOps.h`, and `graphCrud.h`, and link with pkg-config:

```sh
gcc example.c $(pkg-config --cflags --libs graphdata)
```

A graph is a `graph_t` plus a `graphops_t` function table. `GRAPH_OK` means the call succeeded. `LINKED` and `HASHED` take a `NULL` dimensions pointer. `ARRAY`, `SHARED_MEM`, and `SHARED_MMAP` need one from `createDimensions`, whose first argument is the number of dimensions.

```c
struct graph_t *g = initGraph(LINKED | DIRECTED | GENERIC, 0, NULL);
struct graphops_t *ops = getOperations(g);

size_t id = 10;
ops->addNode(&id, g);

struct node_t *nodes = ops->listNodes(g);
/* walk nodes; each entry carries nodeid only */
destroyNodes((void **)&nodes);

destroyGraphops((void **)&ops);
clearGraph(g);
destroyGraph((void **)&g);
```

`getNode`, `getNeighbors`, and `listNodes` return caller-owned node chains. Release them with `destroyNodes`. `getEdge` and `getEdges` return caller-owned edge chains. Release them with `destroyEdges`. `clearGraph` frees the backing store. `destroyGraph` frees the `graph_t` shell and does not free dimensions or labels. Release those with `destroyDimensions` and `destroyLabels`.

`graphDataLib/examples` is a breadth-first shortest path on all five backends. It is also the `bfs_demo` test. Against an install, the same sources build with the pkg-config line above.

## Contributing

Pull requests target `trunk`. From `graphDataLib`, `./buildLinux.sh -t` is the check that has to pass.

No IDE project files are required. The command line above is enough to build and test. Editor settings stay on your machine.

Code that uses this library includes the four installed headers and links with pkg-config. It does not include `impl/` or `util/`.
