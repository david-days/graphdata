# AI and tool-assisted contributions

This policy and guidance is for both human contributors and AI agents working in these repositories.

## Preamble

This note exists because gc-nav is built with substantial help from coding
assistants and other generative tools. That is intentional; AI tooling can have
a significant positive impact on productivity and progress. The question is
not whether those tools are allowed; they exist, are used, and will continue to be
used for the foreseeable future. The question is how these tools affect code and
documentation for all future users, both human and synthetic.

The stance here largely follows the Linux kernel community’s
guidance on tool-generated and AI-assisted work, and especially Linus
Torvalds’ framing:

- do not invent stupid rules that ban particular tools
- expect people to understand what they are doing
- that understanding has very little to do with which tools they used

Kernel process docs make the same practical demands in more detail—human responsibility
for the submission, no machine certifying origin, transparency when a meaningful
amount of content was not written by the human who owns the change, and a hard
bar on untested or unexplainable patches.

gc-nav is a personal program, not the kernel. We're not copying kernel DCO
ceremony, cover-letter culture, or commit-trailer archaeology. But we are adopting the
substance: tools are welcome; owning the result is not optional.

## Scope

This policy applies to work under the gc-nav umbrella and its component repositories
(graphdata, graphAlg, and later packages), including agent-assisted sessions.

Trivial tooling is out of scope for special disclosure: identifier completion,
mechanical renames, formatters, and small spelling or grammar fixes. When in
doubt, prefer a short honest note over silence.

## Policy

1. **Human owns every change that lands.** The person who approves the commit
   or merge must understand the change well enough to explain and defend it.
   If that is not true, do not land (commit/push/merge) it until that is true.

2. **Agents do not certify authorship.** Commit tags identifying tooling contributions
   clutter up the git log. If tool identification is needed, include a line in the commit
   message.

   Prefer a normal sentence in the commit body or PR text; do not add structured trailers
   (`Signed-off-by:`, `Assisted-by:`, `Co-authored-by:`, …) unless asked.

   Commit signing remains the human’s domain, and all commits should be defendable by the human
   contributor.

3. **Test before commit.** After a meaningful change set, prefer one solid
   component test run (e.g. `./buildLinux.sh -t`). Agent-managed runs should state
   explicitly what was not built or verified.

4. **Human-initiated contributions.** Unless overridden by local user policy, agents should
   not push or open PRs without human approval.

5. **Optional transparency.** For non-trivial AI- or tool-generated content,
   a brief note in the PR description, session handoff, or umbrella docs is
   enough (what helped, which areas, how it was checked). No AI trailer in git history—
   a missing tag is not a reliable signal for authorship.

## References (upstream)

- [Kernel Guidelines for Tool-Generated Content](https://docs.kernel.org/process/generated-content.html)
- [AI Coding Assistants](https://docs.kernel.org/process/coding-assistants.html)
