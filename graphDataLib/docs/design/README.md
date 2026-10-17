# graphData design notes

Working decisions for the public ABI and package boundaries. Gather and implement
these (with tests) in graphData before introducing a separate graphAlg package.

| Note | Status | Summary |
|------|--------|---------|
| [001-getter-ownership-always-copy](001-getter-ownership-always-copy.md) | Accepted; LINKED/HASHED getters copy | Public getters return caller-owned snapshots |
| [002-iterators-and-scratch](002-iterators-and-scratch.md) | Implemented | Backend iterators under `impl/`; public list + foreach; scratch stays in Alg |

## Package target (notional)

- `graphData` / `graphData-devel` — storage + `graphops_t` ABI
- `graphAlg` / `graphAlg-devel` — algorithms; depends only on public graphData
- Later: IO/translation, stats/display, domain packages

See individual notes for contracts that keep that split honest.
