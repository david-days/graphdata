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

| Backend | Structure | Id space | Growth | Undirected `getNeighbors` | Generally best for… |
| --- | --- | --- | --- | --- | --- |
| `LINKED` | Doubly linked nodes + per-node edge lists | Arbitrary | Add nodes/edges freely | Not symmetric (min→max store) | Modest sparse graphs; size unknown up front |
| `HASHED` | Hash table of those node chains | Arbitrary | Add + rehash | Symmetric (both directions) | Larger sparse graphs; frequent lookup by id |
| `ARRAY` | Fixed adjacency / cap / flow arrays | `0 .. n-1` | Fixed at init | Not symmetric (min→max slots) | Grids / dense graphs with known size and max degree |
| `SHARED_MEM` | ARRAY layout in SysV shared memory | Same as ARRAY | Fixed at init | Same as ARRAY | Same-machine process(es) sharing one live graph |
| `SHARED_MMAP` | ARRAY layout in mapped files | Same as ARRAY | Fixed at init | Same as ARRAY | Graph that should persist on disk and be mapped again |

### Choosing a backend

- **Sparse + arbitrary ids, in-process:** `LINKED` or `HASHED`. For neighbor-walk algorithms (BFS/DFS/CC), prefer `HASHED|UNDIRECTED`, or `LINKED|DIRECTED` with both arcs — not bare `LINKED|UNDIRECTED` (see [Undirected edge storage](#undirected-edge-storage)).
- **Fixed index + fixed degree:** `ARRAY` (and the shared variants below when you need IPC or durability).
- **Same machine, share one ARRAY-shaped graph:** `SHARED_MEM` — short note below; details in [docs/shared-mem.md](docs/shared-mem.md).
- **On-disk ARRAY-shaped graph:** `SHARED_MMAP` — short note below; details in [docs/shared-mmap.md](docs/shared-mmap.md).

There is no single “best” backend; the public `graphops_t` API is the same. Pick the layout that matches id space, growth, and how you walk neighbors.

### Shared backends (short)

Both shared types reuse the **ARRAY** layout (fixed ids, fixed degree, same undirected neighbor rules and `edgeCount` meaning). They differ in *where* the arrays live and how a second view attaches.

- **`SHARED_MEM`** — SysV shared-memory segments for nodes, capacity, and flow. Creator process builds a writable graph; a read-only view attaches to the same keys. Segments vanish when the last (owner) teardown removes them — not a durable store. See **[docs/shared-mem.md](docs/shared-mem.md)**.
- **`SHARED_MMAP`** — file-backed `mmap` of the same arrays. Suitable when the graph should survive process exit and be mapped again from known paths. See **[docs/shared-mmap.md](docs/shared-mmap.md)**.

`RD_ONLY` is valid only on these two backends. `attachReadonlyGraph` (in `graphInit.h`) is the current in-process helper for a second read-only `graph_t` on the same backing store; multi-process discovery/handshake is still provisional — details in the linked docs.

## Initialization

`initGraph` takes a flag combination, a label count, and an optional dimensions pointer. Omit a group and that group uses its default. Passing no flags yields `UNDIRECTED | LINKED | GENERIC | UNLABELED`.

That default is convenient for small CRUD demos, but it is a poor default for neighbor-walk algorithms: `LINKED|UNDIRECTED` does not give symmetric `getNeighbors` (see below). Prefer an explicit combo such as `LINKED | DIRECTED | GENERIC` (as in the Usage snippet), and add both arcs when you need undirected traversal on LINKED.

| Group | Options | Default | Notes |
| --- | --- | --- | --- |
| Direction | `UNDIRECTED`, `DIRECTED` | `UNDIRECTED` | Directed edges are stored as given. Undirected storage differs by backend (see below) |
| Backend | the five above | `LINKED` | Choose one. Naming two backends is rejected |
| Domain | `GENERIC`, `SPATIAL` | `GENERIC` | Stored on the graph. This version does not change the layout from it |
| Labels | `UNLABELED`, `LABELED` | `UNLABELED` | `LABELED` requires a label count greater than zero |
| Access | omit, or `RD_ONLY` | writable | `RD_ONLY` is accepted only on `SHARED_MEM` and `SHARED_MMAP` |

`ARRAY`, `SHARED_MEM`, and `SHARED_MMAP` require a dimensions pointer. `LINKED` and `HASHED` take `NULL`. On the array-layout backends, `LABELED` also multiplies the node count by the label count.

### Undirected edge storage

`UNDIRECTED` does not mean the same physical layout on every backend:

| Backend | Undirected edge record | `getNeighbors` / `getEdges` |
| --- | --- | --- |
| `LINKED` | One record on `min(u,v)` → `max(u,v)` | Not symmetric: only the endpoint that owns the record lists the other |
| `ARRAY` (and shared array layouts) | Same min→max slot convention | Same asymmetry for neighbor iteration |
| `HASHED` | Both directions stored | Symmetric neighbor lists |

Pairwise edge APIs (`getEdge`, `setCapacity`, `removeEdge`, and related) canonicalize undirected endpoints, so either argument order works for “is there an edge between these two?” Traversal algorithms that walk `getNeighbors` as undirected incidence need either a backend that stores both directions (`HASHED`), or `DIRECTED` with both arcs added explicitly (common for `LINKED`).

### `edgeCount`

`ops->edgeCount(g)` counts **stored records** (or slots), not a portable “number of logical undirected links”:

| Backend | What `edgeCount` returns |
| --- | --- |
| `LINKED` | Number of `edge_t` records (undirected: one per link) |
| `HASHED` | Number of directed adjacency records (undirected: **two** per link) |
| `ARRAY` / `SHARED_*` | Length of the adjacency/cap/flow arrays (slot capacity), not how many slots are occupied |

Do not compare `edgeCount` across backends as if it meant the same thing, and do not assume `edgeCount == 1` after one undirected `addEdge` on HASHED.

## Building

The library is C, built with CMake 3.25 or newer. Compiler warnings are errors (`-Wall -Werror`). The repository root is the CMake project root.

```sh
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
/* Explicit DIRECTED: safer for getNeighbors walks than the UNDIRECTED|LINKED default */
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

Public getters are **always-copy**: you own the snapshots and release them with `destroyNodes` / `destroyEdges`. Graph teardown (`clearGraph` / `destroyGraph`) and dimensions/labels are separate. Full table, empty-vs-OOM on `listNodes`, and common mistakes: **[docs/ownership.md](docs/ownership.md)**.

`examples/` is a breadth-first shortest path on all five backends. It is also the `bfs_demo` test. Against an install, the same sources build with the pkg-config line above.

## Contributing

Pull requests target `trunk`. `./buildLinux.sh -t` is the check that has to pass.

No IDE project files are required. The command line above is enough to build and test. Editor settings stay on your machine.

Code that uses this library includes the four installed headers and links with pkg-config. It does not include `impl/` or `util/`.
