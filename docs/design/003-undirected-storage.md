# 003 — Undirected edge storage vs neighbor iteration

**Status:** Accepted as-is; documented (no layout change).  
**Applies to:** LINKED, ARRAY (and SHARED_* array layouts), HASHED  
**Related:** graphData README “Undirected edge storage”; `graphOps.h` getNeighbors;
gc-nav `docs/design/undirected-neighbor-asymmetry.md`

## Decision

Keep backend-specific undirected layouts:

| Backend | Physical undirected record | `getNeighbors` / `getEdges` |
|---------|----------------------------|-------------------------------|
| LINKED | One edge on `min(u,v)` → `max(u,v)` | Not symmetric |
| ARRAY / SHARED_* | Same min→max slot convention | Not symmetric |
| HASHED | Both directions | Symmetric |

Pairwise edge APIs canonicalize undirected endpoints. Neighbor/edge-list
iteration does not invent the reverse incidence.

## Why

LINKED has unbounded degree and no fixed index space; dual-storing every
undirected link doubles list traffic. ARRAY has a fixed degree budget; one
slot per logical undirected edge matches that budget. HASHED already keys
adjacency by endpoint, so dual-store matches lookup-by-u naturally.

## Consumer rule

- Edge CRUD / capacity / flow by `(u,v)`: either order on undirected LINKED/ARRAY.
- Traversal / degree-from-neighbors / CC: use HASHED undirected, or DIRECTED
  with both arcs (especially on LINKED).
- `edgeCount` counts **records** (HASHED undirected: 2 per link; LINKED: 1).
  ARRAY returns adjacency array length (slots), not occupied-edge count.

## Non-goals

- Changing LINKED/ARRAY undirected to dual-store.
- A separate “undirectedNeighbors” API in this pass (could be proposed later).
