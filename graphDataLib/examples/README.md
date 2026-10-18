# BFS example

One unweighted shortest-path search, run on all five backends. The graph and the expected answer stay fixed, so a difference in the printout is a difference in the library, not in the algorithm.

```
10 -> 20 -> 50 -> 70 -> 90
10 -> 30 -> 70
30 -> 50
```

The shortest path from 10 to 90 is `10 -> 30 -> 70 -> 90`, distance 3.

| File | Role |
| --- | --- |
| `bfs_demo.c` | Builds that graph on each backend, runs the search, and checks the path |
| `bfs_shortest_path.c` | The search. It includes only the public headers and keeps its own workspace |

## Build

From `graphDataLib`, `./buildLinux.sh -t` compiles the example and runs it as the `bfs_demo` test. A passing run prints one line per backend:

```
LINKED: 10 -> 30 -> 70 -> 90 (distance 3)
HASHED: 10 -> 30 -> 70 -> 90 (distance 3)
ARRAY: 10 -> 30 -> 70 -> 90 (distance 3)
SHARED_MEM: 10 -> 30 -> 70 -> 90 (distance 3)
SHARED_MMAP: 10 -> 30 -> 70 -> 90 (distance 3)
```

Against an install, the same two sources need nothing from this tree:

```sh
gcc bfs_demo.c bfs_shortest_path.c $(pkg-config --cflags --libs graphdata)
```

## Setup

`main` creates one dimensions value, `createDimensions(2, 10, 10)`, and passes it only to the array-layout backends. That is a fixed index of 100 nodes, `0 .. 99`, with two adjacency slots per node. The count `2` is the degree. Nodes 10 and 30 each have two outgoing edges, so two slots are enough and three would be wasted.

`LINKED` and `HASHED` receive `NULL` for dimensions. `loadGraph` adds the six ids, then the seven edges. On `ARRAY` and the shared backends those ids already exist, and `addNode` returns `GRAPH_OK` without storing anything. The edges are what matter. Each edge is stored with capacity `1.0`. The search never reads capacity.

Each backend is a separate graph:

```c
struct graph_t *g = initGraph(impl | DIRECTED | GENERIC, 0, dims);
struct graphops_t *ops = getOperations(g);
```

`impl` is one of `LINKED`, `HASHED`, `ARRAY`, `SHARED_MEM`, or `SHARED_MMAP`. The label count is `0` because the graph is unlabeled.

## Search

`bfsShortestPath` does not know which backend it was given. The steps are:

1. `listNodes` copies the ids that exist. The copy is a `node_t` chain of ids only. The search copies those ids into its own array and releases the chain with `destroyNodes`.
2. A queue, a seen flag, a distance, and a parent are allocated beside the graph, one slot per id. `SIZE_MAX` means a node has no parent yet.
3. The queue starts at node 10. For each node taken off the queue, `getNeighbors` returns the outgoing ids. That list is also caller-owned and is released with `destroyNodes` before the next node.
4. The first time the search reaches 90, it stops. Walking `parent` from 90 back to 10 rebuilds `10, 30, 70, 90`.

The same function is the piece to replace or extend. A different cost, a different stop rule, or an undirected walk belongs there. The demo's job is only to feed it five graphs and compare the path with `kExpect`.

## Teardown

Each run releases its own graph, in this order:

```c
destroyGraphops((void **)&ops);
clearGraph(g);
destroyGraph((void **)&g);
```

`clearGraph` frees the backing store. `destroyGraph` frees the `graph_t` shell. The dimensions value is shared by the three array-layout runs, so `main` calls `destroyDimensions` once, after those runs return.

## Adapting it

Change `kNodes`, `kEdges`, and `kExpect` to point the same search at another directed graph. On an array-layout backend, raise the first argument of `createDimensions` when any node needs more outgoing slots than that number. Drop the `runOne` calls for backends you do not need. Leave the queue and the parent links in your own code. The library does not keep them.
