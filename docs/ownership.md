# Ownership and release: getters, lists, and graph lifetime

**Audience:** anyone calling the installed `graphData` API who needs a clear
answer to “what did I just get, and how do I free it?”

**Related:** design note
[`001-getter-ownership-always-copy`](../graphDataLib/docs/design/001-getter-ownership-always-copy.md)
(why always-copy); undirected neighbor asymmetry in the
[README](../README.md#undirected-edge-storage) and
[`003-undirected-storage`](../graphDataLib/docs/design/003-undirected-storage.md);
shared backends [SHARED_MEM](shared-mem.md) / [SHARED_MMAP](shared-mmap.md).

## Rule in one line

Public structure getters return **caller-owned snapshots**. You release them
with the matching `destroy*` helper. Mutating a snapshot does **not** change
the live graph.

## Quick reference

| You called | You own | Release with | Notes |
| --- | --- | --- | --- |
| `getNode` | one `node_t` (nodeid only) | `destroyNodes` | `NULL` if absent |
| `getNeighbors` | node chain of neighbor ids | `destroyNodes` | `NULL` if absent or no stored neighbors |
| `listNodes` | node chain of present ids | `destroyNodes` | see [Empty vs failure](#empty-vs-failure-on-listnodes) |
| `getEdge` | one `edge_t` (u, v, cap, flow) | `destroyEdges` | `NULL` if absent |
| `getEdges` | edge chain from that node | `destroyEdges` | `NULL` if absent or no edges |
| `getCapacity` / `getFlow` | nothing (writes `*cap` / `*flow`) | — | live scalar read |
| `setCapacity` / `addCapacity` / `setFlow` / `addFlow` | nothing | — | live scalar write |
| `getOperations` | `graphops_t` shell | `destroyGraphops` | does not destroy the graph |
| `initGraph` | `graph_t` shell + backing | `clearGraph` then `destroyGraph` | see [Graph lifetime](#graph-lifetime) |
| `createDimensions` / labels | those objects | `destroyDimensions` / `destroyLabels` | not freed by `destroyGraph` |

Use the helpers in `graphCrud.h`. Do not `free()` a getter result by hand, and
do not assume a returned `node_t` / `edge_t` is a live pointer into the graph.

## Always-copy means

1. **Safe to destroy** — releasing a snapshot must not corrupt the graph on any
   backend (LINKED, HASHED, ARRAY, SHARED_MEM, SHARED_MMAP).
2. **No write-through** — changing fields on a returned `node_t` or `edge_t`
   does not update the live structure. Capacity and flow changes go through
   the scalar ops only.
3. **One rule for algorithms** — portable code (including graphAlg) can assume
   always-copy and must not `#include` `impl/` for “borrowed” internals.

A future peek/borrow API, if added, will be **separate and explicit**. It will
not silently replace these getters.

## Empty vs failure on `listNodes`

`listNodes` returns `NULL` in two different situations:

| Situation | `listNodes` | How to tell |
| --- | --- | --- |
| Empty graph (no nodes) | `NULL` | `nodeCount(g) == 0` |
| Allocation failure while nodes exist | `NULL` | `nodeCount(g) > 0` |

Treat “`NULL` and `nodeCount > 0`” as failure (typically OOM). Do not assume
every `NULL` means “empty.”

`foreachNode` on an empty graph still returns `GRAPH_OK` (zero visits).

## Graph lifetime

Typical teardown order:

```c
destroyGraphops((void **)&ops);   /* ops shell only; graph untouched */
clearGraph(g);                    /* free backing store */
destroyGraph((void **)&g);        /* free graph_t shell */
/* if you created them: */
destroyDimensions((void **)&dims);
destroyLabels((void **)&labels);
```

- `clearGraph` frees the backend’s nodes/edges/arrays (and related impl state).
- `destroyGraph` frees the `graph_t` shell. It does **not** free a
  `dimensions_t` or label block you passed into `initGraph`; those stay
  caller-owned.
- Destroying a `listNodes` / `getNeighbors` chain does **not** remove live
  nodes from the graph.

## Common mistakes

- Calling `free()` on a getter result instead of `destroyNodes` /
  `destroyEdges` (skips proper chain teardown).
- Forgetting to destroy a list after a successful `getNeighbors` /
  `listNodes` (leak).
- Assuming `LINKED|UNDIRECTED` neighbor lists are symmetric — ownership is
  fine; **incidence** may not be. See the README undirected section.
- Comparing or interpreting `edgeCount` as “logical undirected links” across
  backends — it counts stored records or slots; see the README `edgeCount`
  section.

## graphAlg

Algorithm result buffers (`graphAlgPathResult`, `graphAlgComponentsResult`,
and similar) follow the same always-copy idea: on success the caller owns the
buffers; release with the matching `*Destroy` helper documented on that type.
graphAlg must only use the four installed graphData headers and pkg-config
`-lgraphData`.
