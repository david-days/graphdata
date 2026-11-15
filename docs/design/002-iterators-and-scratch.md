# 002 — Iterators and scratch space

**Status:** Accepted and implemented (list + foreach on `graphops_t`)  
**Applies to:** public graphData traversal contract; backend iterators under `impl/`  
**Related:** [001-getter-ownership-always-copy](001-getter-ownership-always-copy.md)  
**Blocks:** portable graphAlg (needs backend-agnostic node enumeration)

## Decision

### Iteration

- Each graph backend implements iteration under `impl/` (ARRAY, LINKED, HASHED,
  SHARED_MEM, SHARED_MMAP), wired through the public API — not by exposing
  `impl/` headers to graphAlg.
- The public contract provides portable node enumeration in one or both forms
  (both preferred for thoroughness and future-proofing):
  - **List:** caller-owned sequence of `nodeid` values (always-copy / always
    owned; release via public destroy helper).
  - **Foreach:** blind callback over nodes (`nodeid` + consumer context);
    no cursor object in the v1 surface.
- Existing adjacency walks remain `getNeighbors` / `getEdges` (caller-owned
  lists per note 001). Node enumeration fills the gap that `nodeCount()` alone
  cannot: LINKED/HASHED ids are not necessarily dense `0 .. n-1`.

### Scratch space

- graphData remains **structure-state** focused: topology, cap/flow, labels,
  dims, shared attach metadata.
- **Algorithm ephemeral data and ephemeral states** (visited bits, BFS queues,
  parent maps, distances, residual bookkeeping) live in the algorithm consumer
  (graphAlg or application) — never on `graph_t`, nodes, or `metaImpl` as a
  back door for Alg.
- Paths, stacks, and queues built from graph elements continue to use
  clone/list patterns already documented on `cloneNode` / `cloneEdge`.

### Deferred (planned, not v1)

- **Cursor** — explicit “where am I” iterator object with internal position
  tracking.
- **Peek** — zero-copy / internally managed views (see note 001 escape hatch).

These may be added later as separately named APIs without changing the default
list/foreach + always-copy contract.

## Context

Today there is no node iterator on `graphops_t`. Consumers can pull adjacency
lists, but cannot portably enumerate all nodes on sparse/arbitrary-id backends.
There is also no scratch facility — which is intentional for a substrate
library, but must be stated so graphAlg does not push visit state into Data.

## Consequences

### graphData (to implement before graphAlg)

1. Add per-backend iteration under `impl/` (private find/walk over array slots,
   link chains, hash buckets, shared mirrors).
2. Expose public list and foreach node enumeration on `graphops_t`
   (`listNodes`, `foreachNode`). Backend walks stay in `impl/`. `listNodes`
   is built from the backend foreach walk. Release the list with
   `destroyNodes` in `util/crudops.h` (that header stays out of the install
   set until packaging finalizes public headers).
3. Unit tests across backends:
   - Enumeration visits each present nodeid exactly once.
   - List form is caller-owned; destroy does not remove live nodes.
   - Foreach sees the same set as the list form.
   - Works on LINKED/HASHED with non-dense ids, not only ARRAY.
4. Document that algorithm scratch is out of scope for graphData.

### graphAlg (downstream rule)

- Traverse nodes via public enumeration; traverse adjacency via
  `getNeighbors` / `getEdges` (or future cursor/peek if adopted).
- Allocate and free all ephemeral algorithm state itself (or accept
  caller-supplied workspace buffers).
- Must not require write access to the graph for scratch — RO shared attach
  remains usable for read-only analysis if values are not mutated.

### Non-goals (v1)

- Cursor objects and peek/borrow iteration (deferred, planned).
- Storing visit/parent/queue state inside graph structures.
- Requiring dense `nodeid` space for portable algorithms.

## Related package boundary

graphAlg depends only on public graphData headers and the enumeration +
getter contract. Backend iterators stay under `impl/`; the package split stays
honest if Alg never includes those headers.
