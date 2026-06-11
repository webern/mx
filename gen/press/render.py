"""Render a target: expand its manifest, press its templates, write files.

The whole emit stage, language-free. The manifest (config [render]) declares
which template renders which plate strategies into which output pattern;
this module expands it (with fail-loud checks), builds contexts
(gen.press.context), renders through the engine (gen.press.engine), runs the
target's optional post-render format command in a scratch directory, and
hands the result to the writer (write-if-changed, marker-gated pruning).
"""

from __future__ import annotations

import shutil
import subprocess
import tempfile
from pathlib import Path

from gen.config import Config, RenderEntry
from gen.plates.model import Plates
from gen.press.context import plate_context, target_context
from gen.press.engine import Press, PressError
from gen.press.writer import EmitResult, is_generated, write_files

# The strategy vocabulary, for typo-gating manifest entries (one template
# per shape, the original design principle, now literally one file per
# strategy or group of strategies).
_STRATEGIES = frozenset(
    {
        "enum-class", "numeric-wrapper", "string-wrapper", "tagged-variant",
        "value-class", "composite-class", "flag", "attrs-class",
        "inherit", "flatten",
    }
)


class RenderError(Exception):
    pass


def render_target(plates: Plates, config: Config) -> EmitResult:
    if config.render is None:
        raise RenderError(f"config has no [render] manifest: {config.path}")
    if config.output_dir is None:
        raise RenderError(f"config has no [output] dir: {config.path}")
    files = render_files(plates, config)
    if config.render.format:
        files = _run_format(files, config.render.format)
    return write_files(config.output_dir, files)


def render_files(plates: Plates, config: Config) -> dict[str, str]:
    """The full manifest expansion: every output path with its rendered
    content. Pure (no filesystem writes); the CLI's debugging command and
    the tests use it directly."""
    manifest = config.render
    _check_strategies(manifest.types)

    all_plates = list(plates.value_types) + list(plates.complex_types)
    _check_coverage(manifest.types, all_plates)
    overridden = _check_type_rows(manifest.types, all_plates)

    # Expand every output path first: collisions are manifest bugs and must
    # be reported before any rendering, and once-templates receive the
    # complete output list as context.
    outputs: list[tuple[str, RenderEntry, object]] = []  # (path, entry, plate|None)
    for entry in manifest.types:
        for plate in all_plates:
            if entry.types:
                selected = plate.name.wire in entry.types
            else:
                # Type rows override: a bespoke type never falls through to
                # its strategy's stock template.
                selected = (
                    plate.strategy in entry.strategies
                    and plate.name.wire not in overridden
                )
            if selected:
                outputs.append((_expand(entry.output, plate), entry, plate))
    for entry in manifest.once:
        outputs.append((entry.output, entry, None))
    _check_output_collisions(outputs)

    press = Press(partials=_loader(manifest.dir))
    paths = [path for path, _, _ in outputs]
    files: dict[str, str] = {}
    for path, entry, plate in outputs:
        if plate is not None:
            context = plate_context(plates, plate)
        else:
            context = target_context(plates, paths)
        template = _template_text(manifest.dir, entry.template)
        content = press.render(template, context, entry.template)
        if not is_generated(content):
            raise RenderError(
                f"{entry.template}: output carries no generated-file marker; "
                f"print {{{{generated_banner}}}} in a comment near the top"
            )
        files[path] = content
    return files


def _loader(directory: Path):
    def load(name: str) -> str:
        # Partial references use bare names: {{> fields}} loads fields.tmpl.
        if "." not in name:
            return _template_text(directory, name + ".tmpl")
        return _template_text(directory, name)

    return load


def _template_text(directory: Path, name: str) -> str:
    path = (directory / name).resolve()
    if not str(path).startswith(str(directory)):
        raise RenderError(f"template path escapes the templates directory: {name}")
    if not path.is_file():
        raise PressError(name, 0, "no such template")
    return path.read_text(encoding="utf-8")


def _expand(pattern: str, plate) -> str:
    """Output patterns compose file names from the plate's casings:
    `mx_{snake}.h`, `{pascal}.go`. Unknown placeholders fail loud."""
    out = pattern
    for convention, ident in plate.name.cased.items():
        out = out.replace("{" + convention + "}", ident)
    if "{" in out or "}" in out:
        raise RenderError(
            f"output pattern {pattern!r} has an unknown placeholder "
            f"(known: {', '.join(sorted(plate.name.cased))})"
        )
    return out


def _check_strategies(entries: list[RenderEntry]) -> None:
    for entry in entries:
        unknown = set(entry.strategies) - _STRATEGIES
        if unknown:
            raise RenderError(
                f"[render.type] {entry.template}: unknown strategies "
                f"{sorted(unknown)} (known: {', '.join(sorted(_STRATEGIES))})"
            )


def _check_type_rows(entries: list[RenderEntry], all_plates: list) -> set[str]:
    """Validate the bespoke rows and return the wire names they claim. A
    name no plate carries is a stale manifest entry and fails loud."""
    known = {p.name.wire for p in all_plates}
    claimed: set[str] = set()
    for entry in entries:
        unknown = set(entry.types) - known
        if unknown:
            raise RenderError(
                f"[render.type] {entry.template}: 'types' names no plate: "
                f"{sorted(unknown)}"
            )
        claimed.update(entry.types)
    return claimed


def _check_coverage(entries: list[RenderEntry], all_plates: list) -> None:
    """With any per-type entries present, every plate must be rendered by at
    least one -- a strategy silently falling through the manifest would drop
    types from the output."""
    if not entries:
        return  # a once-only target (e.g. a single-document schema)
    covered = {s for e in entries for s in e.strategies}
    missed = sorted({p.strategy for p in all_plates} - covered)
    if missed:
        raise RenderError(
            f"the render manifest covers no template for strategies: {missed}"
        )


def _check_output_collisions(outputs: list[tuple[str, RenderEntry, object]]) -> None:
    """Output paths must be unique case-insensitively (one file per path on
    macOS/Windows), across type-expanded and once entries alike."""
    claims: dict[str, list[str]] = {}
    for path, entry, plate in outputs:
        claimant = plate.name.wire if plate is not None else f"once:{entry.template}"
        claims.setdefault(path.lower(), []).append(f"{claimant} -> {path}")
    problems = {k: v for k, v in claims.items() if len(v) > 1}
    if problems:
        lines = [
            f"output path collision (case-insensitive): {', '.join(sorted(set(v)))}"
            for v in problems.values()
        ]
        raise RenderError("\n".join(sorted(lines)))


def _run_format(files: dict[str, str], command: tuple[str, ...]) -> dict[str, str]:
    """The target's post-render formatter (e.g. gofmt), run once over a
    scratch directory before the writer's idempotence diff. `{dir}` expands
    to the scratch path. The command is target data; the generator knows
    only run-this-and-fail-loud."""
    if shutil.which(command[0]) is None:
        raise RenderError(f"[render] format command not found on PATH: {command[0]}")
    with tempfile.TemporaryDirectory() as scratch:
        root = Path(scratch)
        for rel, content in files.items():
            target = root / rel
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(content, encoding="utf-8")
        expanded = [arg.replace("{dir}", scratch) for arg in command]
        result = subprocess.run(expanded, capture_output=True, text=True)
        if result.returncode != 0:
            raise RenderError(
                f"[render] format command failed ({result.returncode}): "
                f"{result.stderr.strip() or result.stdout.strip()}"
            )
        return {
            rel: (root / rel).read_text(encoding="utf-8") for rel in files
        }
