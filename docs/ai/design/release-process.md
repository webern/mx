# Release process design

Status: PROPOSED. This is the design record for `mx`'s GitHub Actions release system. It was
produced from a deep-research pass on how C++/Swift libraries do continuous delivery on GitHub,
followed by an owner interview. The decisions below are settled; the workflows themselves are
follow-up work.

## 1. Goal

Ship binary releases of `mx` for multiple package ecosystems from a chosen commit, with a
provable guarantee that what gets published is exactly what was built and validated — never a
silent rebuild from a different tree. Swift (a binary XCFramework consumed via SwiftPM) is the
first release target. Conan is a planned second target; the architecture must not be Swift-only.

The design deliberately assembles this from GitHub primitives rather than copying a precedent:
the deep-research pass found no real-world project running this exact "build a SHA, then a tag
verifies that same SHA" two-phase pattern. Most projects do single-phase (tag-triggered build
+ publish, or a `workflow_dispatch` version-bump). The two-phase split exists here to make the
"published bytes == validated bytes" invariant airtight.

## 2. Human touchpoints

The entire release is four manual actions. Everything else is automation reacting to events.

1. **(Optional) Open and merge a PR** for any real work the release needs. → commit `A` on `main`.
2. **Run `release-build`** (a `workflow_dispatch`), passing SHA `A`.
3. **Review and merge the auto-generated `release-prep` PR** the build opened. → merge commit `B`.
4. **Push a tag** (`vMAJOR.MINOR.PATCH`) pointing at `B`.

No other human step exists. In particular there is no "click publish on a draft," no manual
checksum edit, and no reviewer-approval gate beyond the tag push itself — pushing the tag *is*
the approval.

`release-build` can be triggered three interchangeable ways, since `workflow_dispatch` is one
API event: the GitHub Actions UI ("Run workflow" → `sha` field), the `gh` CLI
(`gh workflow run release-build.yml -f sha=<40-hex>`), or the GitHub MCP tool
(`actions_run_trigger`, `method: run_workflow`, `inputs: { sha }`). In all three, the dispatch
`ref` selects only *which copy of the workflow YAML runs* (normally `main`); the commit actually
built is the `sha` input, which the job checks out explicitly.

## 3. The checksum constraint (why `B` exists)

SwiftPM resolves a version by checking out the *tagged commit* and reading `Package.swift` as it
exists there. For binary-release mode to work for a consumer depending on `from: "1.4.0"`, the
`Package.swift` at the `v1.4.0` commit must already contain the correct
`binaryTarget(url:checksum:)`. The URL is predictable ahead of time (it is a fixed function of
the tag name and a fixed asset filename). The **checksum is not** — it can only be computed after
the XCFramework is built.

A commit cannot contain the hash of its own future build output; that is a property of content
hashing, not a fixable limitation. Therefore *some* commit must exist between "the SHA the owner
picked" (`A`) and "the SHA that gets tagged" (`B`), carrying the checksum that `A`'s build
produced. That commit is `B`, introduced by the `release-prep` PR.

**All such release-metadata edits are bundled into the single `release-prep` PR.** If a future
target (Conan) has an analogous "the tagged commit must already carry a computed value" gotcha,
its fix goes into the same PR diff — not a second PR. The `release-prep` PR is the one and only
place release metadata is written back into the tree.

Because `B` differs from `A` only in release-metadata files (never `src/`), `A`'s build output is
byte-identical to what `B` would produce. The system relies on this: it does **not** rebuild at
`B`; it re-associates `A`'s already-built artifacts with `B` (§5).

## 4. Workflows

All new, all separate from `ci.yaml`. Names are indicative.

| File | Trigger | Responsibility |
|---|---|---|
| `release-build.yml` | `workflow_dispatch`, input `sha` (required; validated against `^[0-9a-f]{40}$`) | Checks out `sha` explicitly. Calls one reusable target workflow per release target. Uploads each target's artifacts (`retention-days: 90`). Opens the `release-prep` PR (branch `release-prep/<sha>`, label `release-prep`) bundling every target's metadata patch. |
| `_release-target-swift.yml` | `workflow_call` | Builds three slices via CMake + an iOS toolchain file: macOS universal (`arm64`+`x86_64`), iOS device (`arm64`), iOS simulator (`arm64`+`x86_64`). Stitches them with `xcodebuild -create-xcframework`, zips, computes the SHA256 via `swift package compute-checksum`. Outputs: artifact name, checksum, the `Package.swift` patch, and the list of files it is allowed to touch. |
| `_release-target-conan.yml` | `workflow_call` | *Future.* Same input/output contract as the Swift target (SHA in; artifact + metadata patch + touched-file list out). Not built now; its existence in this table is the proof the abstraction is not Swift-only. |
| `release-prep-merged.yml` | `pull_request: closed`, filtered to `merged == true` **and** label `release-prep` | Parses `A` from the branch name. Hard-fails unless the `A..B` diff touches *only* the union of files each target declared. Downloads `release-build`'s artifact for `A` by run ID and **re-uploads it as this run's own artifact**, now keyed to `B`. This run's success is the validated record for `B`. |
| `release-publish.yml` | `push: tags: ["v*"]` | Resolves the tag's commit SHA. Queries the Actions API for the most recent **successful `release-prep-merged` run** with that `head_sha`. Hard-fails if none. Downloads that run's re-uploaded artifact and publishes a GitHub Release immediately (not draft), `generate_release_notes: true`, attaching the zip and its `.sha256`. |

## 5. The SHA-binding invariant

Phase 2 (`release-publish`) must publish exactly the bytes phase 1 validated. Two design choices
enforce this:

- **No rebuild.** `release-publish` never compiles anything. It fetches an already-built artifact
  by workflow-run ID and publishes it verbatim. This removes rebuild drift as a possible failure
  mode entirely. (Build-provenance attestation — `actions/attest-build-provenance`, SLSA/Sigstore
  — was evaluated and rejected for this role: the research found no confirmed mechanism by which
  it proves phase 2's artifacts equal phase 1's. Artifact re-download by run ID is the reliable
  mechanism.)
- **The lookup key is `head_sha`, and the run queried is `release-prep-merged`, not
  `release-build`.** This is the subtle part: `release-build` ran against `A`, but the commit that
  gets tagged is `B`. Only `release-prep-merged` ever ran against `B`. Its success — gated on the
  `A..B` diff being release-metadata-only — *is* the authoritative "this SHA is validated" record.
  No separate database, manifest, or attestation is needed; the Actions run history is the source
  of truth, and it cannot be forged by the workflow itself.

If a SHA has multiple successful `release-prep-merged` runs (e.g. a re-run), the **most recent**
one is authoritative.

## 6. Artifacts and retention

`release-build` uploads, per target, as a single named artifact (`mx-release-<sha>`): the built
archive (the XCFramework zip), its `.sha256` file, and a small `manifest.json` recording the SHA
and the platforms/arches built. Retention is set explicitly to **`retention-days: 90`** rather
than inherited from the repo default, so the window is a documented contract.

`release-prep-merged` re-uploads that artifact under `B`, starting a fresh 90-day clock keyed to
the taggable commit. 90 days from build to tag is more than sufficient for this project's cadence.

## 7. Versioning and tags

The **git tag is the sole source of truth** for the version. Format `vMAJOR.MINOR.PATCH` (e.g.
`v1.4.0`). There is no `VERSION` file and no version field in `Package.swift` (SwiftPM does not
need one). `release-publish` derives the version by stripping the `v` from `github.ref_name`. A
future Conan target consumes that same string.

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

`Package.swift` today has a stubbed `MX_BINARY_RELEASE` branch that `fatalError`s. This system
lights it up: in binary-release mode it becomes
`.binaryTarget(name: "Mx", url: <release-asset-url>, checksum: <computed>)`. The URL/checksum are
exactly what the `release-prep` PR writes into the tree at `B`.

## 9. The release-target contract (Conan honesty)

Each release target is a reusable workflow (`on: workflow_call`) — reusable workflows, not
composite actions, because only reusable workflows can contain multiple jobs (a build matrix plus
packaging). The shared contract that keeps the system multi-target:

- **Inputs:** the commit `sha` to build.
- **Outputs:** (a) a named artifact containing the publishable archive(s) + checksums + a
  manifest; (b) a metadata patch to apply to the tree in the `release-prep` PR; (c) the list of
  files that patch is allowed to touch (used by `release-prep-merged` to bound the `A..B` diff).

`release-build` fans out across targets and merges every target's metadata patch into the one
`release-prep` PR; `release-prep-merged` validates the diff against the union of all targets'
touched-file lists; `release-publish` publishes every target's artifact under the one tag. Adding
Conan is: write `_release-target-conan.yml` to that contract, register it in the fan-out. No
change to the publish or validation logic. Conan's actual recipe/upload mechanics are deliberately
left unspecified here.

## 10. Token permissions (least privilege, per job)

| Job | Permissions |
|---|---|
| `_release-target-*` build jobs | `contents: read` |
| `release-build` PR-opening job | `contents: write`, `pull-requests: write` |
| `release-prep-merged` | `contents: read` (re-uploads an artifact; does not write to `main`) |
| `release-publish` | `contents: write` (the only job that can create a Release) |

No workflow requests broad repo-wide write. The single job that can mint a public Release is
isolated behind the tag-push trigger.

## 11. Failure modes

- **`release-publish` finds no validated record for the tagged SHA** (wrong SHA tagged, the
  `release-prep` PR was closed-not-merged or lost its label, or the artifact expired past 90
  days): **hard-fail, loudly, no fallback and no auto-rebuild.** The message directs the owner to
  verify the `release-prep` PR merged with its label intact, and to recover by deleting the tag,
  fixing the cause, and re-pushing. Publishing is all-or-nothing; a partial publish is never
  emitted.
- **`release-prep-merged` finds the `A..B` diff touches files outside the declared set:**
  hard-fail. This is the guard that keeps `B`'s tree equal to `A`'s for build purposes; if it
  trips, the artifact reuse would be unsound, so the release is blocked rather than published from
  mismatched bytes.
- **No manual override / `force_rebuild` escape hatch exists**, by design — it would silently
  reintroduce the rebuild-drift risk the whole two-phase split exists to prevent.

## 12. Out of scope / future work

- Conan target implementation (§9 defines only its contract).
- tvOS/watchOS/visionOS slices.
- Code signing / notarization — not applicable to a static-library XCFramework; would only matter
  if `mx` later ships a signed executable.
- Auto-generated release notes are GitHub's default categorization of merged PRs since the prior
  tag; a curated `CHANGELOG.md`-driven flow is not adopted for v1.
