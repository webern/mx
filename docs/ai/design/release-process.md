# Release process design

Status: PROPOSED. This is the design record for `mx`'s GitHub Actions release system. It was
produced from a deep-research pass on how C++/Swift libraries do continuous delivery on GitHub,
followed by an owner interview, then revised after a design review found mechanical flaws in the
first draft (§13 records each flaw and its fix). The decisions below are settled; the workflows
themselves are follow-up work.

## 1. Goal

Ship binary releases of `mx` for multiple package ecosystems from a chosen commit, with a
provable guarantee that what gets published is exactly what was built and validated — never a
silent rebuild from a different tree. Swift (a binary XCFramework consumed via SwiftPM) is the
first release target. Conan is a planned second target; the architecture must not be Swift-only.

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
3. **Review release commit `B`** — the tip of branch `release-prep/v<version>`, which the build
   pushed. The run summary prints `B`'s SHA and links the `A...B` compare view; the diff is
   release metadata only.
4. **Push a tag** (`vMAJOR.MINOR.PATCH`) pointing at `B`.

No other human step exists. In particular there is no PR to merge (§3 explains why `B` never
lands on `main`), no "click publish on a draft," no manual checksum edit, and no
reviewer-approval gate beyond the tag push itself — pushing the tag *is* the approval, and the
review in step 3 is what the tag pusher is vouching for.

`release-build` can be triggered three interchangeable ways, since `workflow_dispatch` is one
API event: the GitHub Actions UI ("Run workflow" → `sha` and `version` fields), the `gh` CLI
(`gh workflow run release-build.yml -f sha=<40-hex> -f version=1.4.0`), or the GitHub MCP tool
(`actions_run_trigger`, `method: run_workflow`, `inputs: { sha, version }`). In all three, the
dispatch `ref` selects only *which copy of the workflow YAML runs* (normally `main`); the commit
actually built is the `sha` input, which the job checks out explicitly.

## 3. The checksum constraint (why `B` exists — and why it is not on `main`)

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
produced. That commit is `B`: a single commit whose parent is `A`, created and pushed by
`release-build` on the machine-owned branch `release-prep/v<version>`.

**`B` is never merged into `main`.** Two independent reasons force this:

- *The tagged manifest and `main`'s manifest need opposite defaults.* A consumer's SwiftPM
  evaluates `Package.swift` in the consumer's own environment; there is no way to make every
  downstream consumer set an environment variable, so the tagged commit must resolve to the
  `binaryTarget` **by default**. Meanwhile `main` must default to a source build for
  sibling-checkout development and the non-Apple CI consumers. One file cannot default both ways
  at once, so the binary-default manifest may only ever exist on release commits (§8).
- *Merging would break the artifact-reuse premise.* If anything landed on `main` between `A` and
  the merge, the merge commit's tree would be "current `main` + metadata," not "`A` + metadata,"
  and reusing `A`'s build output for it would be unsound. A merge-based design silently requires
  `main` to be frozen for the duration of every release; building `B` directly on top of `A` on
  its own branch removes that requirement entirely — `main` can move freely at any time.

Being off-`main` is invisible to consumers: SwiftPM resolves versions from tags alone, and a git
tag keeps `B` permanently reachable even if the release branch is later deleted.

**All release-metadata edits from all targets are bundled into the single commit `B`.** If a
future target (Conan) has an analogous "the tagged commit must already carry a computed value"
gotcha, its files go into the same commit — not a second commit. `B` is the one and only place
release metadata is written into a tree.

Because `B` differs from `A` only in declared release-metadata files (never `src/`), `A`'s build
output is byte-identical to what a build at `B` would produce. The system relies on this: it
does **not** rebuild at `B`; it publishes `A`'s already-built artifacts for `B` (§5).

## 4. Workflows

All new, all separate from `ci.yaml`. Names are indicative.

| File | Trigger | Responsibility |
|---|---|---|
| `release-build.yml` | `workflow_dispatch`, inputs `sha` (required; validated against `^[0-9a-f]{40}$`) and `version` (required; validated against `^\d+\.\d+\.\d+$`) | Checks out `sha` explicitly. Calls one reusable target workflow per release target. Uploads each target's artifacts (`retention-days: 90`). A final prep job collects every target's metadata files, writes `release-manifest.json` (§5), commits `B` on `release-prep/v<version>` (force-push; the branch is machine-owned), and prints `B`'s SHA plus the `A...B` compare link in the run summary. |
| `_release-target-swift.yml` | `workflow_call` | Builds three slices via CMake + an iOS toolchain file: macOS universal (`arm64`+`x86_64`), iOS device (`arm64`), iOS simulator (`arm64`+`x86_64`). Stitches them with `xcodebuild -create-xcframework`, zips, computes the SHA256 via `swift package compute-checksum`. Outputs: artifact name, checksum, the complete release-mode `Package.swift` content (§8), and the list of paths it is allowed to occupy in `B`. |
| `_release-target-conan.yml` | `workflow_call` | *Future.* Same input/output contract as the Swift target (SHA + version in; artifact + metadata files + allowed-path list out). Not built now; its existence in this table is the proof the abstraction is not Swift-only. |
| `release-publish.yml` | `push: tags: ["v*"]` | Resolves the tag to `B`, reads `release-manifest.json` from `B`'s tree, and re-verifies every claim in it (§5). Hard-fails on any mismatch. Otherwise downloads the pinned run's artifacts and publishes a GitHub Release immediately (not draft), `generate_release_notes: true`, attaching each target's archive and its `.sha256`. |

The first draft had a fourth workflow, `release-prep-merged.yml`, reacting to the merge of a
release-prep PR; with no PR and no merge, it no longer exists.

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
  2. `B` has exactly one parent and it is the manifest's `A`;
  3. `A` is an ancestor of `main` (releases come from mainline history only);
  4. the `A..B` diff touches only the declared paths — additionally clamped by a static ceiling
     hardcoded in `release-publish.yml` itself (release metadata paths only; never `src/`,
     never `.github/`);
  5. the pinned run exists **in this repository**, is a run of `release-build.yml`, and
     completed successfully;
  6. its artifacts download, and a freshly recomputed SHA-256 of each archive equals both the
     manifest's checksum and the checksum string inside `B`'s `Package.swift` (for the Swift
     target). For a zip, `swift package compute-checksum` *is* the SHA-256 of the file, so plain
     `sha256sum` on the publish runner reproduces it.

Why not query the Actions run history by commit, as the first draft did? Two API facts make that
unsound: for a `pull_request`-triggered run, the Actions API reports `head_sha` as the *PR
branch head*, never the merge commit — so the draft's lookup ("the validating run whose
`head_sha` is the tagged SHA") could never match anything; and a `workflow_dispatch` run cannot
be located by its `sha` *input* at all (inputs are not queryable, and such a run's `head_sha` is
the dispatch ref's head, not the input). Pinning the run ID inside `B` solves both discovery
problems with no heuristics — including the draft's "most recent successful run wins"
tie-break, which is gone: re-dispatching the same version force-pushes a fresh `B`, and the tag
pins exactly one.

Residual trust, stated honestly: someone with push access could hand-craft a `B` (including,
since tag-push workflows execute the YAML at the tagged commit, a `B` that tampers with
`release-publish.yml` itself — which check 4 forbids, but a tampered workflow would not enforce
check 4). Checks 1–6 make such a commit fail loudly against any honest workflow copy; what
closes the loop is that nothing publishes until an admin reviews the `A...B` diff (human step 3,
where a workflow edit is glaring) and pushes the protected tag. That is the same trust anchor
the first draft placed in "review and merge the PR," relocated, not weakened.

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

## 8. Build mechanics (Swift target)

`mx` builds with CMake/Make, not an Xcode project, and the design keeps it that way. Compilation
uses CMake with an iOS toolchain file (e.g. `leetal/ios-cmake`) invoked once per slice; CMake
drives `clang`/`clang++` **directly** (the same compiler the existing native `macos` CI job
uses) — no `.xcodeproj` is generated and no `xcodebuild` runs for *compilation*. The toolchain
file only points CMake at the correct Apple SDK sysroot and target triple per slice.

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

Two `Package.swift` manifests exist in this design, and only one ever lives on `main`:

- **`main`'s manifest** stays a source-only package: sibling-checkout development
  (`.package(path:)`) and any non-Apple consumption keep working with zero setup. The env-gated
  `MX_BINARY_RELEASE` arm that `fatalError`s today is superseded by this design and should be
  deleted when the workflows land — `main` never switches to binary mode, so the arm is dead
  code under the final scheme.
- **The release manifest**, generated by `_release-target-swift.yml` from a template checked in
  at `A` (`.github/release/Package.swift.template`, with the URL and checksum interpolated) and
  existing only in `B`: `.binaryTarget(name: "Mx", url: <release-asset-url>, checksum:
  <computed>)` is the **default**, so a `from: "1.4.0"` consumer gets the binary with no
  environment setup — the fix for the first draft's fatal assumption that consumers could be
  asked to set an environment variable. The source target remains reachable from the release
  manifest as the automatic arm on non-Apple hosts (where an XCFramework `binaryTarget` cannot
  resolve; the manifest is Swift code evaluated on the consumer's machine, so `#if os(...)`
  handles this) and behind an explicit `MX_SOURCE_BUILD=1` opt-out for anyone who wants to
  compile a tagged release from source on a Mac.

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
one commit `B`; `release-publish` validates the diff against the union of all targets' declared
paths (clamped by its static ceiling, §5) and publishes every target's artifact under the one
tag. Adding Conan is: write `_release-target-conan.yml` to that contract, register it in the
fan-out, and add its metadata paths to the publish ceiling. No other change to the publish or
validation logic. Conan's actual recipe/upload mechanics are deliberately left unspecified here.

## 10. Token permissions (least privilege, per job)

| Job | Permissions |
|---|---|
| `_release-target-*` build jobs | `contents: read` |
| `release-build` prep job | `contents: write` (pushes the `release-prep/v*` branch; no PR is opened, so no `pull-requests` scope) |
| `release-publish` | `contents: write` (the only job that can create a Release), `actions: read` (reads the pinned run's metadata and downloads its artifacts — cross-run artifact access requires it; the first draft's table omitted `actions: read` entirely, so both of its artifact-reading jobs would have been denied by the API) |

No workflow requests broad repo-wide write. The single job that can mint a public Release is
isolated behind the tag-push trigger.

## 11. Failure modes

- **Any `release-publish` verification failure** (§5: missing or malformed manifest — including
  a tag pushed at `A` or any non-release commit; tag/version mismatch; parentage, ancestry, or
  diff violation; pinned run missing, failed, or foreign; artifact expired past 90 days;
  checksum mismatch): **hard-fail, loudly, no fallback and no auto-rebuild.** Publishing is
  all-or-nothing; a partial publish is never emitted. The failure message names the check that
  tripped and directs the owner to recover by deleting the tag, fixing the cause (re-dispatching
  `release-build` when the artifact expired or the run was bad — this produces a fresh `B` to
  tag), and re-pushing.
- **Commits landing on `main` between `A` and the tag:** harmless. `B` sits directly on `A` on
  its own branch, so mainline activity cannot leak into the release tree. (In the first draft
  this was a silent freeze requirement that would have hard-failed any release with a concurrent
  merge.)
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

**2026-08-23 — design review of the first draft found seven flaws; all are fixed above.**

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
   source-only (§3, §8).
4. *The merge-based flow silently required a frozen `main`.* Any commit landing between `A` and
   the release-prep merge made the merged tree diverge from `A` + metadata. → `B` is a single
   commit atop `A` on a release branch, never merged (§3).
5. *The permissions table omitted `actions: read`,* which cross-run artifact download requires;
   both artifact-reading jobs would have 403'd. → Added (§10).
6. *Nothing specified how the `release-build` run ID was discovered* (dispatch inputs are not
   queryable via the API). → The run pins its own `github.run_id` into `B`'s manifest (§5).
7. *The written checksum was never verified against the artifact,* so a corrupted or tampered
   checksum would ship a release every consumer fails to resolve. → Publish recomputes the
   SHA-256 and requires bytes == manifest == `Package.swift` (§5, check 6).

Also: per-target artifact names now include the target (`mx-release-<target>-<sha>`) so a second
target cannot collide with Swift's artifact key (§6).
