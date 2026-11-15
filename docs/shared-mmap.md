# SHARED_MMAP — array layout in memory-mapped files

**Audience:** choosing or wiring `SHARED_MMAP` graphs.  
**Related:** [README backends](../README.md#available-backends), [SHARED_MEM](shared-mem.md), [ownership](ownership.md), ARRAY undirected rules in [003-undirected-storage](../graphDataLib/docs/design/003-undirected-storage.md).

## What it is

`SHARED_MMAP` is the **ARRAY** adjacency/cap/flow layout backed by **memory-mapped files** (paths for node, capacity, and flow data). Node ids, degree, and undirected storage match ARRAY: fixed `0 .. n-1`, degree from `dimensions_t`, undirected edges as min→max slots.

Use it when the graph should **remain on disk** and be mapped again later (or shared via known file paths), not only held in anonymous shared memory for the life of a creator process.

## Requirements

- Pass a non-NULL `dimensions_t` to `initGraph` (same as ARRAY).
- Optional `LABELED` multiplies the node count by the label count (same as ARRAY).
- `RD_ONLY` is accepted on this backend; it is rejected for LINKED/HASHED/ARRAY.

## Creator vs read-only view

| Role | How | Backing |
| --- | --- | --- |
| Writable owner | `initGraph(SHARED_MMAP \| …, …, dims)` without `RD_ONLY` | Creates/opens mapped files and maps them read/write |
| Read-only view | `attachReadonlyGraph(src)` on a writable `SHARED_MMAP` graph | Remaps the same paths with `PROT_READ` |

`attachReadonlyGraph` is **provisional**: in-process helper for policy and tests. It is **not** the long-term multi-process reader API (no path publication protocol, handshake, or versioning beyond “same paths as the creator’s meta”). Treat path discovery and multi-process attach as deferred.

## Dimensions lifetime

Same as SHARED_MEM: the read-only graph **shares** `src->dims`. Do not destroy dims until both graphs are cleared. Free the read-only graph before the writable owner.

## Teardown

Writable teardown may unlink the backing files (owner cleans up). Read-only teardown unmaps without unlinking, so the owner still controls file lifetime. Coordinate so readers are gone before the owner removes files you still need on disk — or design so files are meant to persist and only the owner unlinks when retiring the dataset.

## Implications for algorithms

- Same as ARRAY for growth, id space, undirected `getNeighbors`, and `edgeCount`.
- Write ops on `RD_ONLY` fail via ops policy / read-only mapping.
- Public consumers (including graphAlg) use installed headers only — not `impl/sharedmmap*`.

## When to prefer SHARED_MEM instead

Choose [SHARED_MEM](shared-mem.md) for **same-machine, non-durable** sharing with SysV segments and no file paths to manage. Prefer `SHARED_MMAP` when durability or remapping from disk matters.
