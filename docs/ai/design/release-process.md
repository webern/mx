# Release process design

Status: IMPLEMENTED. This is the design record for `mx`'s GitHub Actions release system. It was
produced from a deep-research pass on how C++/Swift libraries do continuous delivery on GitHub,
followed by an owner interview, then revised twice: once after a design review found mechanical
flaws in the first draft, and once for the owner requirement that release tags live on `main`
(§13 records each change and why). The workflows in §4 exist and this document is kept in sync
with them; §13's implementation entry records where the build drifted from the design text.

## 1. Goal

Ship binary releases of `mx` for multiple package ecosystems from a chosen commit, with a
provable guarantee that what gets published is exactly what was built and validated — never a
silent rebuild from a different tree. Swift (a binary XCFramework consumed via SwiftPM) is the
first release target. Conan is a planned second target; the architecture must not be Swift-only.
Release tags are part of `main`'s history (owner requirement): every published version is
reachable from `main`, so `git log`, `git describe`, and `git tag --merged main` all see it.

The design deliberately assembles this from GitHub primitives rather than copying a precedent:
the deep-research pass found no real-world project running this exact two-phase pattern (phase 1
builds a chosen SHA and writes a tag-ready release commit; phase 2's tag push re-verifies that
commit from primary sources and publishes). Most projects do single-phase (tag-triggered build
+ publish, or a `workflow_dispatch` version-bump). The two-phase split exists here to make the
"published bytes == validated bytes" invariant airtight.

## 2. Human touchpoints

The entire release is four manual actions. Everything else is automation reacting to events.

1. **(Optional) Open and merge a PR** for any real work the release needs. → commit `A` on `main`.
2. **Run `release-build`** (a `workflow_dispatch`), passing SHA `A` and the version
   `MAJOR.MINOR.PATCH`.
3. **Review and merge the auto-opened release PR** (branch `release-prep/v<version>`), using
   **"Create a merge commit"** — not squash or rebase. §3 explains why the merge method matters;
   §5 enforces it.
4. **Push a tag** (`vMAJOR.MINOR.PATCH`) pointing at commit `B` — the first of the PR's two
   commits, whose SHA the run summary and the PR body both print alongside the exact
   `git tag`/`git push` commands.

No other human step exists. In particular there is no "click publish on a draft," no manual
checksum edit, and no reviewer-approval gate beyond the tag push itself — pushing the tag *is*
the approval, and the review in step 3 is what the tag pusher is vouching for.

`release-build` can be triggered three interchangeable ways, since `workflow_dispatch` is one
API event: the GitHub Actions UI ("Run workflow" → `sha` and `version` fields), the `gh` CLI
(`gh workflow run release-build.yml -f sha=<40-hex> -f version=1.4.0`), or the GitHub MCP tool
(`actions_run_trigger`, `method: run_workflow`, `inputs: { sha, version }`). In all three, the
dispatch `ref` selects only *which copy of the workflow YAML runs* (normally `main`); the commit
actually built is the `sha` input, which the job checks out explicitly.

## 3. The checksum constraint (why `B` exists, and where it sits)

SwiftPM resolves a version by checking out the *tagged commit* and reading `Package.swift` as it
exists there. For binary-release mode to work for a consumer depending on `from: "1.4.0"`, the
`Package.swift` at the `v1.4.0` commit must already contain the correct
`binaryTarget(url:checksum:)`. The URL is predictable ahead of time — it is a fixed function of
the tag name and a fixed asset filename, which is exactly why the version must be supplied at
build time (§7). The **checksum is not** — it can only be computed after the XCFramework is
built.

A commit cannot contain the hash of its own future build output; that is a property of content
hashing, not a fixable limitation. Therefore *some* commit must exist between "the SHA the owner
picked" (`A`) and "the SHA that gets tagged" (`B`), carrying the checksum that `A`'s build
produced.

**The shape that puts `B` on `main` without putting the binary manifest on `main`:** the release
PR that `release-build` opens carries exactly two machine-written commits on branch
`release-prep/v<version>`, based on `A`:

- **`B`** (parent: `A`) — writes the binary-default release `Package.swift` (§8) and
  `release-manifest.json` (§5). This is the commit the tag points at.
- **`R`** (parent: `B`) — restores `Package.swift` to `A`'s copy verbatim
  (`git checkout A -- Package.swift`; purely mechanical, no target involvement), keeping
  `release-manifest.json` as a visible release record on `main`.

Merging the PR with a merge commit `M` makes `B` part of `main`'s history — the same way every
commit of every merged feature branch is on `main` — while `main`'s *tree* changes only by the
release-record file. This resolves the conflict that sank both earlier drafts: a consumer's
SwiftPM evaluates `Package.swift` in the consumer's own environment, and there is no way to make
every downstream consumer set an environment variable, so the tagged commit must resolve to the
`binaryTarget` **by default**; meanwhile every tip of `main` must default to a source build for
sibling-checkout development and the non-Apple CI consumers. One file cannot default both ways
at one commit — so the binary default exists at exactly one commit, `B`, and `R` closes it out
before the branch meets `main`.

Two properties fall out of `B` having parent `A` (rather than being the merge or squash commit,
as in the first draft):

- *No frozen `main`.* Whatever lands on `main` between `A` and the merge, `B`'s tree is still
  exactly "`A` + declared release metadata," so reusing `A`'s build artifacts for `B` stays
  sound. Concurrent merges can at most conflict on `release-manifest.json`, which nothing else
  touches. (The first draft tagged the merge commit itself, which silently required `main` to be
  frozen for the duration of every release.)
- *The PR's net diff is tiny.* GitHub's "Files changed" view (merge-base `A` to head `R`) shows
  only `release-manifest.json`; the binary manifest appears in the commits tab at `B`, which the
  run summary and PR body link directly for review of the URL/checksum lines.

**The merge method is load-bearing.** Only a true merge commit preserves `B` in `main`'s
ancestry. A squash or rebase merge copies the net diff instead — harmless to `main`'s tree (the
net diff is the record file), but `B` would then be reachable only via the tag, violating the
tags-on-`main` requirement. `release-publish` hard-fails in that case (§5, check 3); recovery is
`git merge -s ours release-prep/v<version>` pushed to `main` — a history-only merge whose tree
change is nil, since `main` already carries the net diff — then re-push the tag.

**All release-metadata edits from all targets are bundled into the single commit `B`.** If a
future target (Conan) has an analogous "the tagged commit must already carry a computed value"
gotcha, its files go into `B` too — not a second commit or PR. `B` is the one and only place
release metadata is written into a tree.

Because `B` differs from `A` only in declared release-metadata files (never `src/`), `A`'s build
output is byte-identical to what a build at `B` would produce. The system relies on this: it
does **not** rebuild at `B`; it publishes `A`'s already-built artifacts for `B` (§5).

## 4. Workflows

All new, all separate from `ci.yaml`. Names are indicative.

| File | Trigger | Responsibility |
|---|---|---|
| `release-build.yml` | `workflow_dispatch`, inputs `sha` (required; validated against `^[0-9a-f]{40}$`) and `version` (required; validated against `^\d+\.\d+\.\d+$`) | Checks out `sha` explicitly. Calls one reusable target workflow per release target. Uploads each target's artifacts (`retention-days: 90`). A final prep job collects every target's metadata files, writes `release-manifest.json` (§5), commits `B` then `R` on `release-prep/v<version>` (force-push; the branch is machine-owned), opens the release PR, and prints `B`'s SHA, the tag commands, and the `A...B` compare link in the run summary and the PR body. |
| `_release-target-swift.yml` | `workflow_call` | Builds three slices via CMake + an iOS toolchain file: macOS universal (`arm64`+`x86_64`), iOS device (`arm64`), iOS simulator (`arm64`+`x86_64`). Stitches them with `xcodebuild -create-xcframework`, zips, computes the SHA256 via `swift package compute-checksum`. Outputs: artifact name, checksum, the complete release-mode `Package.swift` content (§8), and the list of paths it is allowed to occupy in `B`. |
| `_release-target-conan.yml` | `workflow_call` | *Future.* Same input/output contract as the Swift target (SHA + version in; artifact + metadata files + allowed-path list out). Not built now; its existence in this table is the proof the abstraction is not Swift-only. |
| `release-publish.yml` | `push: tags: ["v*"]` | Resolves the tag to `B`, reads `release-manifest.json` from `B`'s tree, and re-verifies every claim in it (§5). Hard-fails on any mismatch. Otherwise downloads the pinned run's artifacts and publishes a GitHub Release immediately (not draft), `generate_release_notes: true`, attaching each target's archive and its `.sha256`. |

No workflow reacts to the release PR's merge; merging only places `B` into `main`'s history, and
all validation happens at tag time. (The first draft had a `release-prep-merged.yml` for this;
it is gone, along with its label machinery.)

**Implementation notes.** Each target hands its results to the prep job through a companion
artifact, `mx-release-<target>-metadata-<sha>`: a `tree/` directory holding the exact files to
commit in `B` (for Swift, the interpolated release `Package.swift`) plus `fragment.json` (target
name, artifact name, archive filename, checksum, platforms, allowed paths) — the concrete form
of the §9 contract's outputs (b) and (c). The prep job merges every target's `tree/`, builds
`release-manifest.json` from the fragments, and refuses to commit if anything outside the
declared paths is dirty. The prep job pushes the branch and opens the PR with the workflow's own
`GITHUB_TOKEN`, and GitHub suppresses workflow triggers for events created with that token — so
**the release PR runs no CI**. That is acceptable by design: CI validated `A` on its own PR,
`release-build` built and checksummed `A`, and the release PR's content is machine-written
metadata. Two repository settings are prerequisites: **"Allow GitHub Actions to create and
approve pull requests"** (Settings → Actions → General) must be on, and **"Create a merge
commit"** must be an allowed merge method (§3).

One bootstrap consequence of the `push: tags` trigger: GitHub runs the workflow YAML as it
exists *at the tagged commit* — i.e. `B`, whose tree is `A` plus metadata. So only SHAs that
already contain `release-publish.yml` are releasable. Acceptable; no escape hatch needed.

## 5. The SHA-binding invariant

Phase 2 (`release-publish`) must publish exactly the bytes phase 1 validated. Two design choices
enforce this:

- **No rebuild.** `release-publish` never compiles anything. It fetches an already-built artifact
  by workflow-run ID and publishes it verbatim. This removes rebuild drift as a possible failure
  mode entirely. (Build-provenance attestation — `actions/attest-build-provenance`,
  SLSA/Sigstore — was evaluated and rejected for this role: the research found no confirmed
  mechanism by which it proves phase 2's artifacts equal phase 1's. Artifact download by run ID
  is the reliable mechanism.)
- **`B` carries its own provenance, and publish re-derives every claim from primary sources.**
  `release-manifest.json`, committed in `B` by the prep job, records: the version, the base SHA
  `A`, the `release-build` **run ID** (`github.run_id` is available to a run while it executes,
  so the run that builds the artifacts can pin itself into `B`), each target's artifact name and
  checksum, and the union of paths the targets declared they may touch. `release-publish` trusts
  none of it blindly; at tag time it hard-fails unless **all** of the following hold:
  1. the tag name equals `v` + the manifest's version (so the asset URL baked into
     `Package.swift` can never desync from the tag actually pushed);
  2. the tagged commit has exactly one parent and it is the manifest's `A` — which also catches
     tagging the wrong commit: `R` fails it (its parent is `B`), as does the merge commit (two
     parents), and `A` itself has no manifest at all;
  3. `A` is an ancestor of `main` (releases come from mainline history only) **and `B` is an
     ancestor of `main`** — the tags-on-`main` guarantee, which requires the release PR to have
     been merged, with a true merge commit, before the tag is pushed (§3 gives the recovery when
     it was squashed);
  4. the `A..B` diff touches only the declared paths — additionally clamped by a static ceiling
     hardcoded in `release-publish.yml` itself (release metadata paths only; never `src/`,
     never `.github/`);
  5. the pinned run exists **in this repository**, is a run of `release-build.yml`, and
     completed successfully;
  6. its artifacts download, and a freshly recomputed SHA-256 of each archive equals both the
     manifest's checksum and the checksum string inside `B`'s `Package.swift` (for the Swift
     target). For a zip, `swift package compute-checksum` *is* the SHA-256 of the file, so plain
     `sha256sum` on the publish runner reproduces it.

`R` and the merge commit are deliberately *not* validated at publish: they affect only `main`'s
tree, which the PR review in step 3 governs.

Why not query the Actions run history by commit, as the first draft did? Two API facts make that
unsound: for a `pull_request`-triggered run, the Actions API reports `head_sha` as the *PR
branch head*, never the merge commit — so the draft's lookup ("the validating run whose
`head_sha` is the tagged SHA") could never match anything; and a `workflow_dispatch` run cannot
be located by its `sha` *input* at all (inputs are not queryable, and such a run's `head_sha` is
the dispatch ref's head, not the input). Pinning the run ID inside `B` solves both discovery
problems with no heuristics — including the draft's "most recent successful run wins"
tie-break, which is gone: re-dispatching the same version force-pushes a fresh branch, and the
tag pins exactly one `B`.

Residual trust, stated honestly: someone with push access could hand-craft a `B` (including,
since tag-push workflows execute the YAML at the tagged commit, a `B` that tampers with
`release-publish.yml` itself — which check 4 forbids, but a tampered workflow would not enforce
check 4). Checks 1–6 make such a commit fail loudly against any honest workflow copy; what
closes the loop is that nothing publishes until an admin reviews the release PR (step 3, where a
workflow edit is glaring) and pushes the protected tag. That is the same trust anchor the first
draft placed in "review and merge the PR" — unchanged, since this design kept that step.

## 6. Artifacts and retention

`release-build` uploads, per target, a single named artifact **`mx-release-<target>-<sha>`**
(e.g. `mx-release-swift-<sha>`; the target name is in the key so a future Conan target cannot
collide with Swift's): the built archive (the XCFramework zip), its `.sha256` file, and a small
manifest fragment recording the SHA and the platforms/arches built. Retention is set explicitly
to **`retention-days: 90`** rather than inherited from the repo default, so the window is a
documented contract.

The first draft re-uploaded artifacts under a second workflow run to re-key them to `B` on a
fresh 90-day clock; with the run ID pinned in `B`'s manifest there is nothing to re-key, and a
single window remains: **the tag must be pushed within 90 days of the build.** That is more than
sufficient for this project's cadence; expiry recovery is §11.

## 7. Versioning and tags

The version is chosen once, at `release-build` dispatch, as a required input. It has to be: the
release-asset URL is a function of the tag name, and it is baked into `B`'s `Package.swift`,
which must exist *before* the tag does. (The first draft called the tag "the sole source of
truth" while also requiring the URL to be written at build time, with no version input anywhere
— a contradiction; no step in its flow ever supplied the version to the thing writing the URL.)

The tag remains the **approval**: nothing publishes until `v<version>` is pushed at `B`, and
`release-publish` hard-fails unless the tag name matches the manifest's version exactly (§5,
check 1), so the tag and the baked URL cannot desync. Format `vMAJOR.MINOR.PATCH` (e.g.
`v1.4.0`). There is still no `VERSION` file and no version field in `Package.swift`;
`release-publish` derives the version string it passes to release notes (and a future Conan
target) from `github.ref_name`.

Release tags are protected by a repository ruleset on pattern `v*`: **Restrict updates** and
**Restrict deletions**, with repo admins on the bypass list. This is complementary to — not a
substitute for — the tag-push-is-approval decision: it prevents *post-hoc tampering* (a
force-push retargeting `v1.4.0` at a different commit after it already shipped), which would
desync the public release from what the tag claims. It does not add a pre-publish review gate.

Once the PR is merged and the tag is pushed, the `release-prep/v<version>` branch can be
deleted: `B` stays reachable from both `main` and the tag.

## 8. Build mechanics (Swift target)

`mx` builds with CMake/Make, not an Xcode project, and the design keeps it that way. Compilation
uses CMake's built-in Apple cross-compilation support, invoked once per slice —
`CMAKE_SYSTEM_NAME=iOS` plus the slice's `CMAKE_OSX_SYSROOT`/`CMAKE_OSX_ARCHITECTURES` (the
design anticipated a third-party toolchain file, e.g. `leetal/ios-cmake`; none turned out to be
needed). CMake drives `clang`/`clang++` **directly** (the same compiler the existing native
`macos` CI job uses) — no `.xcodeproj` is generated and no `xcodebuild` runs for *compilation*.
Each slice builds the `mx` CMake target and its dependencies (`mx_core`, `pugixml`) and merges
the three static libraries into one with `libtool -static`, so each XCFramework slice carries a
single library plus the public headers (`src/include`). No module map ships: the consumers are
C++/ObjC++ and include `mx/api/...` via the header search path SwiftPM derives from the bundle;
Swift-`import` support would be additive later. Deployment floors are macOS 11.0 and iOS 13.0.

The single unavoidable Apple-tool call is the final `xcodebuild -create-xcframework`, used purely
to assemble the three static-lib slices plus headers into a valid `.xcframework` bundle — there
is no vendor-neutral tool that writes that bundle format correctly. Shape:

```
3× (CMake + clang build → static .a + headers)   # macOS universal, iOS device, iOS simulator
1× xcodebuild -create-xcframework                # stitch the three slices
1× zip + swift package compute-checksum          # produce the release artifact + checksum
```

Platform/arch scope for v1: **macOS** (`arm64`+`x86_64`), **iOS device** (`arm64`), **iOS
simulator** (`arm64`+`x86_64`). tvOS, watchOS, and visionOS are explicitly out of scope for v1.

Two `Package.swift` manifests exist in this design, and the binary one lives at exactly one
commit per release:

- **`main`'s manifest** stays a source-only package: sibling-checkout development
  (`.package(path:)`) and any non-Apple consumption keep working with zero setup, at every
  commit of `main` (`R` guarantees this — the binary manifest never survives to a `main` tip).
  The env-gated `MX_BINARY_RELEASE` arm that used to `fatalError` was superseded by this design
  and was deleted when the workflows landed — `main` never switches to binary mode, so the arm
  was dead code under the final scheme.
- **The release manifest**, generated by `_release-target-swift.yml` from a template checked in
  at `A` (`.github/release/Package.swift.template`, with the URL and checksum interpolated) and
  existing only at `B`: `.binaryTarget(name: "Mx", url: <release-asset-url>, checksum:
  <computed>)` is the **default**, so a `from: "1.4.0"` consumer gets the binary with no
  environment setup — the fix for the first draft's fatal assumption that consumers could be
  asked to set an environment variable. The source target remains reachable from the release
  manifest as the automatic arm on non-Apple hosts (where an XCFramework `binaryTarget` cannot
  resolve; the manifest is Swift code evaluated on the consumer's machine, so `#if os(...)`
  handles this) and behind an explicit `MX_SOURCE_BUILD=1` opt-out for anyone who wants to
  compile a tagged release from source on a Mac. The template's source arms carry a copy of the
  source target, so the template must be kept in sync with `Package.swift` whenever the target
  changes — both files carry a KEEP IN SYNC comment pointing at each other, the release PR
  review is the human check, and the build evaluates the interpolated manifest with
  `swift package dump-package` before committing it.

## 9. The release-target contract (Conan honesty)

Each release target is a reusable workflow (`on: workflow_call`) — reusable workflows, not
composite actions, because only reusable workflows can contain multiple jobs (a build matrix plus
packaging). The shared contract that keeps the system multi-target:

- **Inputs:** the commit `sha` to build and the `version` being released.
- **Outputs:** (a) a named artifact `mx-release-<target>-<sha>` containing the publishable
  archive(s) + checksums + a manifest fragment; (b) the exact release-metadata file contents to
  commit in `B`; (c) the list of paths those files may occupy — the target's contribution to the
  allowed-path set that `release-publish` enforces on the `A..B` diff.

`release-build` fans out across targets and its prep job merges every target's files into the
one commit `B` (and mechanically reverts the tree-affecting ones in `R`); `release-publish`
validates the diff against the union of all targets' declared paths (clamped by its static
ceiling, §5) and publishes every target's artifact under the one tag. Adding Conan is: write
`_release-target-conan.yml` to that contract, register it in the fan-out, and add its metadata
paths to the publish ceiling. No other change to the publish or validation logic. Conan's actual
recipe/upload mechanics are deliberately left unspecified here.

## 10. Token permissions (least privilege, per job)

| Job | Permissions |
|---|---|
| `_release-target-*` build jobs | `contents: read` |
| `release-build` prep job | `contents: write` (pushes the `release-prep/v*` branch), `pull-requests: write` (opens the release PR) |
| `release-publish` | `contents: write` (the only job that can create a Release), `actions: read` (reads the pinned run's metadata and downloads its artifacts — cross-run artifact access requires it; the first draft's table omitted `actions: read` entirely, so both of its artifact-reading jobs would have been denied by the API) |

No workflow requests broad repo-wide write. The single job that can mint a public Release is
isolated behind the tag-push trigger.

## 11. Failure modes

- **Any `release-publish` verification failure** (§5: missing or malformed manifest — including
  a tag pushed at `A` or any non-release commit; tag/version mismatch; parentage violation —
  including tagging `R` or the merge commit by mistake; `B` not in `main`'s ancestry — the PR
  unmerged, or squash/rebase-merged; diff violation; pinned run missing, failed, or foreign;
  artifact expired past 90 days; checksum mismatch): **hard-fail, loudly, no fallback and no
  auto-rebuild.** Publishing is all-or-nothing; a partial publish is never emitted. The failure
  message names the check that tripped and directs the owner to recover by deleting the tag,
  fixing the cause (merging the PR when it wasn't; the `-s ours` history-only merge when it was
  squashed, §3; re-dispatching `release-build` when the artifact expired or the run was bad —
  producing a fresh PR and a fresh `B` to tag), and re-pushing.
- **Commits landing on `main` between `A` and the tag:** harmless. `B` sits directly on `A`, so
  mainline activity cannot leak into the tagged tree; at worst the release PR needs a trivial
  conflict resolution in `release-manifest.json`, which nothing else touches. (In the first
  draft this was a silent freeze requirement that would have hard-failed any release with a
  concurrent merge.)
- **No manual override / `force_rebuild` escape hatch exists**, by design — it would silently
  reintroduce the rebuild-drift risk the whole two-phase split exists to prevent.

## 12. Out of scope / future work

- Conan target implementation (§9 defines only its contract).
- tvOS/watchOS/visionOS slices.
- Code signing / notarization — not applicable to a static-library XCFramework; would only matter
  if `mx` later ships a signed executable.
- Auto-generated release notes are GitHub's default categorization of merged PRs since the prior
  tag; a curated `CHANGELOG.md`-driven flow is not adopted for v1.

## 13. Revision log

**2026-08-23 (c) — implemented.** The workflows in §4, the template in §8, and the
`Package.swift` cleanup shipped in one commit. Implementation drift from the design text, each
recorded in place above: CMake's built-in `CMAKE_SYSTEM_NAME=iOS` support replaced the
anticipated third-party toolchain file, and each slice's three static libraries are merged with
`libtool -static` (§8); the §9 contract's outputs (b) and (c) are concretely a per-target
`mx-release-<target>-metadata-<sha>` artifact consumed by the prep job (§4); the release PR runs
no CI because it is pushed with the workflow token (§4); and two repository settings became
explicit prerequisites — Actions may create PRs, and "Create a merge commit" is an allowed merge
method (§4).

**2026-08-23 (b) — owner requirement: tags live on `main`.** The first revision fixed the
"binary-default vs source-default" conflict and the frozen-`main` problem by tagging a commit on
a never-merged release branch. The owner requires release tags to be part of `main`'s history.
Resolution: the release PR returns, now carrying two machine-written commits — `B` (binary
manifest + provenance record; the tag target) and `R` (restores the source manifest) — so a
merge-commit merge places `B` in `main`'s ancestry while every `main` tip stays a source
package (§3). Both original fixes survive: the binary default still exists at exactly one
commit, and `B`'s parent is still `A`, so nothing about `main`'s movement affects the tagged
tree. New consequences, both enforced by §5 check 3: the release PR must be merged with a true
merge commit (squash/rebase would strand `B` outside `main`'s history; recovery documented in
§3), and the tag can only be pushed after the merge. `release-build` regains
`pull-requests: write`.

**2026-08-23 (a) — design review of the first draft found seven flaws; all remain fixed above.**

1. *The `head_sha` lookup could never work.* A `pull_request: closed`-triggered run reports the
   PR branch head as its `head_sha`, never the merge commit, so `release-publish` querying for a
   validating run keyed to the tagged SHA would find nothing, every time. → Run-history queries
   are gone; `B` pins its builder's run ID in `release-manifest.json` and publish re-verifies
   from primary sources (§5).
2. *The version was unknown when the asset URL had to be written.* The tag was "the sole source
   of truth," chosen at step 4, yet the URL derived from it was needed at step 2. → `version` is
   a required `release-build` input; publish enforces tag == manifest version (§7).
3. *Consumers could never reach binary mode.* The env-gated `MX_BINARY_RELEASE` arm is
   unreachable for downstream SwiftPM consumers, who cannot be made to set environment
   variables. → The release manifest, existing only at `B`, is binary-by-default; `main` stays
   source-only at every tip (§3, §8).
4. *The merge-based flow silently required a frozen `main`.* Any commit landing between `A` and
   the release-prep merge made the merged tree diverge from `A` + metadata. → The tagged commit
   is `B`, whose parent is always `A`; the merge commit is never tagged, so `main`'s movement
   is irrelevant to the tagged tree (§3).
5. *The permissions table omitted `actions: read`,* which cross-run artifact download requires;
   both artifact-reading jobs would have 403'd. → Added (§10).
6. *Nothing specified how the `release-build` run ID was discovered* (dispatch inputs are not
   queryable via the API). → The run pins its own `github.run_id` into `B`'s manifest (§5).
7. *The written checksum was never verified against the artifact,* so a corrupted or tampered
   checksum would ship a release every consumer fails to resolve. → Publish recomputes the
   SHA-256 and requires bytes == manifest == `Package.swift` (§5, check 6).

Also: per-target artifact names now include the target (`mx-release-<target>-<sha>`) so a second
target cannot collide with Swift's artifact key (§6).
