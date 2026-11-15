# SHARED_MEM — array layout in SysV shared memory

**Audience:** choosing or wiring `SHARED_MEM` graphs.  
**Related:** [README backends](../README.md#available-backends), [SHARED_MMAP](shared-mmap.md), [ownership](ownership.md), ARRAY undirected rules in [003-undirected-storage](../graphDataLib/docs/design/003-undirected-storage.md).

## What it is

`SHARED_MEM` is the **ARRAY** adjacency/cap/flow layout placed in **System V shared memory** segments (separate segments for node index data, capacity, and flow). Node ids, degree, and undirected storage match ARRAY: fixed `0 .. n-1`, degree from `dimensions_t`, undirected edges as min→max slots.

Use it when multiple contexts on the **same machine** should see one live, fixed-size graph without copying the arrays into each process’s private heap.

## Requirements

- Pass a non-NULL `dimensions_t` to `initGraph` (same as ARRAY).
- Optional `LABELED` multiplies the node count by the label count (same as ARRAY).
- `RD_ONLY` is accepted on this backend; it is rejected for LINKED/HASHED/ARRAY.

## Creator vs read-only view

| Role | How | Backing |
| --- | --- | --- |
| Writable owner | `initGraph(SHARED_MEM \| …, …, dims)` without `RD_ONLY` | Creates SysV segments (`IPC_CREAT \| IPC_EXCL`) and attaches read/write |
| Read-only view | `attachReadonlyGraph(src)` on a writable `SHARED_MEM` graph | Attaches the same keys with `SHM_RDONLY` |

`attachReadonlyGraph` is **provisional**: it is an in-process helper for policy and tests. It is **not** yet a multi-process discovery API (no key publication, handshake, or versioning). Cross-process readers that look up keys themselves are future work; do not treat today’s attach helper as that contract.

## Dimensions lifetime

The returned read-only graph **shares** `src->dims`. Do not `destroyDimensions` until both the writable and read-only graphs have been cleared/destroyed. Free the read-only graph **before** the writable owner.

## Teardown

On `clearGraph` / free of a **writable** owner, shared segments are removed. A **read-only** attach detaches without removing the segments (so the owner still controls destruction). Destroying the owner while readers still need the data will invalidate those mappings — coordinate lifetimes explicitly.

## Implications for algorithms

- Same as ARRAY for growth (none), id space, `getNeighbors` asymmetry, and `edgeCount` (slot capacity).
- Write ops on an `RD_ONLY` graph fail via ops policy (and OS read-only attach where applicable).
- graphAlg and other consumers still use only the public headers; they do not include `impl/sharedmem*`.

## When to prefer SHARED_MMAP instead

Choose [SHARED_MMAP](shared-mmap.md) if the graph must **persist on disk** or be remapped after the creating process exits. `SHARED_MEM` is for live same-machine sharing, not durable storage.
