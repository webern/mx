"""C backend: render the Plates into the C test target sources.

Dumb renderers per the design: identifiers (including composed enum
constants), casings, type mappings, file stems, and structure arrive
resolved on the plates; this package owns only C grammar and the runtime
support files. Each plate's FileId renders as a header/impl pair -- the
documented one-FileId-to-two-files mapping for C.

Alongside the sources, the backend emits `sources.cmake`, the generated
manifest the test target's CMakeLists includes, so the build never globs
and never goes stale.

Rendered: the four value shapes, the four complex shapes, the document
entry points, and the runtime support files.
"""

from __future__ import annotations

from gen.emit.c.complexes import complex_files
from gen.emit.c.document import document_files, document_stem
from gen.emit.c.runtime import runtime_header, runtime_impl, runtime_stem
from gen.emit.c.values import value_files
from gen.emit.writer import banner
from gen.plates.model import Plates


def _guard_identifiers(plates: Plates, reserved_stems: tuple[str, ...]) -> None:
    """The plates' collision gate certifies projected identifiers; this
    backend also COMPOSES a few (the per-type Child struct, the has_/children
    fields, its support types). A schema name landing on one of those must
    fail loud here, not as a confusing compile error in generated code."""
    from gen.plates.model import element_members

    prefix = plates.target.prefix
    reserved_idents = {f"{prefix}Document", f"{prefix}Namespace"}
    type_idents = {p.ident for p in list(plates.value_types) + list(plates.complex_types)}
    for plate in list(plates.value_types) + list(plates.complex_types):
        if plate.file in reserved_stems:
            raise ValueError(
                f"type '{plate.name.wire}' projects to the reserved file stem "
                f"'{plate.file}'; rename it in config.toml"
            )
        if plate.ident in reserved_idents:
            raise ValueError(
                f"type '{plate.name.wire}' projects to '{plate.ident}', which this "
                f"backend reserves; rename it in config.toml"
            )
    for plate in plates.complex_types:
        members = plate.members_view()
        elements = element_members(members)
        if elements and f"{plate.ident}Child" in type_idents:
            raise ValueError(
                f"type '{plate.ident}Child' collides with '{plate.name.wire}'s "
                f"child struct; rename one in config.toml"
            )
        idents = {m.ident for m in members}
        if elements and idents & {"children", "children_count"}:
            raise ValueError(
                f"'{plate.name.wire}' has a member projecting to a reserved "
                f"children field; rename it in config.toml"
            )
        for m in members:
            if m.kind == "attribute" and f"has_{m.ident}" in idents:
                raise ValueError(
                    f"'{plate.name.wire}': member 'has_{m.ident}' collides with "
                    f"the presence flag of attribute '{m.name.wire}'; rename one"
                )


def render(plates: Plates) -> dict[str, str]:
    rt = runtime_stem(plates)
    doc_stem = document_stem(plates)
    _guard_identifiers(plates, (rt, doc_stem, "sources"))
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
    for plate in plates.complex_types:
        header, impl = complex_files(plates, plate, includes_of.get(plate.file, []), rt)
        files[plate.file + ".h"] = header
        files[plate.file + ".c"] = impl
    header, impl = document_files(plates, doc_stem, rt)
    files[doc_stem + ".h"] = header
    files[doc_stem + ".c"] = impl

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
