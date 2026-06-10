"""C backend: render the Plates into the C test target sources.

Dumb renderers per the design: identifiers (including composed enum
constants), casings, type mappings, file stems, and structure arrive
resolved on the plates; this package owns only C grammar and the runtime
support files. Each plate's FileId renders as a header/impl pair -- the
documented one-FileId-to-two-files mapping for C.

Alongside the sources, the backend emits `sources.cmake`, the generated
manifest the test target's CMakeLists includes, so the build never globs
and never goes stale.

Currently rendered: the four value shapes (the leaf node types) plus the
runtime. Complex types and the document entry points are later phases.
"""

from __future__ import annotations

from gen.emit.c.runtime import runtime_header, runtime_impl, runtime_stem
from gen.emit.c.values import value_files
from gen.emit.writer import banner
from gen.plates.model import Plates


def render(plates: Plates) -> dict[str, str]:
    rt = runtime_stem(plates)
    reserved = (rt, "sources")
    for plate in list(plates.value_types) + list(plates.complex_types):
        if plate.file in reserved:
            raise ValueError(
                f"type '{plate.name.wire}' projects to the reserved file stem "
                f"'{plate.file}'; rename it in config.toml"
            )
    includes_of = {
        spec.file: spec.includes for spec in (plates.files or [])
    }

    files: dict[str, str] = {
        f"{rt}.h": runtime_header(plates, rt),
        f"{rt}.c": runtime_impl(plates, rt),
    }
    for plate in plates.value_types:
        header, impl = value_files(plates, plate, includes_of.get(plate.file, []), rt)
        files[plate.file + ".h"] = header
        files[plate.file + ".c"] = impl

    files["sources.cmake"] = _sources_cmake(plates, files)
    return files


def _sources_cmake(plates: Plates, files: dict[str, str]) -> str:
    """The build manifest: every emitted .c, listed explicitly so CMake
    reconfigures exactly when the generator changes the file set."""
    sources = sorted(name for name in files if name.endswith(".c"))
    lines = [f"# {banner(plates.source)}", ""]
    lines += ["set(MX_GENERATED_SOURCES"]
    lines += [f"    ${{CMAKE_CURRENT_LIST_DIR}}/{name}" for name in sources]
    lines += [")"]
    return "\n".join(lines) + "\n"
