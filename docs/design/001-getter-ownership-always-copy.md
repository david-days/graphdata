# 001 — Getter ownership: always-copy

**Status:** Accepted. LINKED and HASHED `getNode` / `getEdge` return clones; mutators use private live lookups. Installed-header placement of `destroyNodes` / `destroyEdges` is still deferred.  
**Applies to:** public `graphops_t` getters in graphData  
**Blocks:** graphAlg package work (consistency + unit tests first)

## Decision

Public structure getters return **caller-owned snapshots**. One ownership rule
for every backend (ARRAY, LINKED, HASHED, SHARED_MEM, SHARED_MMAP):

| Getter | Ownership |
|--------|-----------|
| `getNode` | Caller-owned copy |
| `getEdge` | Caller-owned copy |
| `getNeighbors` | Caller-owned list (already) |
| `getEdges` | Caller-owned list (already) |

- Releasing a snapshot must be safe on all backends (prefer public
  `destroyNode` / `destroyEdge` / list helpers over ad-hoc `free`).
- Mutating fields on a returned `node_t` / `edge_t` does **not** write through
  to the live graph.
- Live capacity/flow updates go through scalar ops only:
  `getCapacity` / `setCapacity` / `addCapacity`,
  `getFlow` / `setFlow` / `addFlow`.

## Context

The historical copy/borrow split was an accident of representation, not a
deliberate dual contract:

- ARRAY (and SHARED_*) store neighbors/cap/flow in parallel arrays. Returning
  `node_t` / `edge_t` required synthesizing graph-speak structures → natural
  **copy**.
- LINKED / HASHED already store live `node_t` / `edge_t` chains → cheap
  **borrow** of in-graph pointers.

Documented in `graphOps.h` as “implementation-specific” for `getNode` /
`getEdge`, while list getters were already “distinct; caller frees.” HASHED
followed LINKED (borrow) but was not called out in the docs.

That split is hostile to a portable graphAlg layer: the same call site must
not `free` a LINK/HASH result and must not assume ARRAY write-through.

## Consequences

### graphData (to implement before graphAlg)

1. Change LINKED and HASHED `getNode` / `getEdge` to return clones (e.g.
   `cloneNode` / `cloneEdge`), matching ARRAY.
2. Keep private find/lookup helpers for internal mutators; do **not** route
   structural edits through the public getters once they clone.
3. Rewrite `graphOps.h` (and related comments) to state always-copy; remove
   implementation-specific ownership wording for these getters.
4. Expose clear public release helpers for snapshots and neighbor/edge lists.
5. Unit tests (all backends that implement the getters):
   - Free/destroy of `getNode` / `getEdge` results does not corrupt the graph.
   - Mutating `cap`/`flow` on a `getEdge` snapshot does not change
     `getCapacity` / `getFlow` on the live edge.
   - After destroy of snapshot, subsequent gets and scalar ops still work.
   - List getters remain caller-owned; destroy list does not remove live edges.

### graphAlg (downstream rule)

Algorithms may assume always-copy on the public API and must not `#include`
`impl/` headers. Prefer scalar ops for value updates; use `getEdges` /
`getNeighbors` for traversal and destroy lists when finished.

### Non-goals (deferred)

Higher throughput with fewer copies is expected long-term. At that time, expose
a **separate or additional** API that explicitly states memory is managed
internally (borrow/peek): do not free; lifetime tied to the graph; invalid
after structural change. That API must not replace or silently change the
default always-copy getters.

## Related package boundary

Alg depends only on public headers (`graphData.h`, `graphOps.h`, `graphInit.h`)
and `-lgraphData`. Always-copy is part of making that dependency honest across
backends without ownership `#ifdef`s.
