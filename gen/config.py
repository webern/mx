"""Load a target's config.toml into a typed Config.

A target config describes one generation run: which schema inputs to read,
where generated code lands, which optional companion patches to apply before
emitting, and how the IR is projected onto the target (the Plates: naming
conventions, renames, type mappings, layout). The IR itself stays a pure
function of the schema inputs (see gen.ir); config selects *which* inputs and
how the result is presented, never what the schema means.

Parsing is structural only: key shapes, types, and the rename addressing
scheme. Semantic validation (does a rename key name something in the IR, do
projected identifiers collide) happens in gen.plates.build, which has the IR
in hand and fails loud there.
"""

from __future__ import annotations

import tomllib
from dataclasses import dataclass, field
from pathlib import Path

from gen.names import CONVENTIONS

# Keys allowed in a rename entry table: a fundamental rename (all casings
# re-expand from the new root) or per-convention overrides (pin one flavor).
_ENTRY_KEYS = frozenset(CONVENTIONS) | {"fundamental"}

# Rename kinds the Plates build consumes today. `group` and `attribute-group`
# are reserved by the design for targets that emit shared fragments/mixins;
# none of ours does, so configuring them is an error rather than a silently
# dead table.
_RENAME_KINDS = ("type", "element", "attribute", "enum-value")


class ConfigError(ValueError):
    """A malformed config file. Always raised with the offending key path."""


@dataclass
class RenameEntry:
    """One rename: an optional fundamental root plus per-convention pins."""

    fundamental: str | None = None
    cased: dict[str, str] = field(default_factory=dict)


@dataclass
class Renames:
    """Parsed [rename.*] tables, keyed by the design's addressing scheme."""

    types: dict[str, RenameEntry] = field(default_factory=dict)
    elements: dict[str, RenameEntry] = field(default_factory=dict)
    attributes: dict[str, RenameEntry] = field(default_factory=dict)  # global
    scoped_attributes: dict[tuple[str, str], RenameEntry] = field(default_factory=dict)
    enum_values: dict[tuple[str, str], RenameEntry] = field(default_factory=dict)

    def __bool__(self) -> bool:
        return bool(
            self.types
            or self.elements
            or self.attributes
            or self.scoped_attributes
            or self.enum_values
        )


@dataclass
class TargetSection:
    symbol_prefix: str = ""  # prepended to type idents and composed constants
    inheritance: bool = True  # derived types: inherit (True) or flatten
    variant_scope: str = "bare"  # constant scoping: "bare" | "composed"


@dataclass
class NamingSection:
    acronyms: tuple[str, ...] | None = None  # None -> the built-in default set
    type_convention: str = "pascal"
    field_convention: str = "snake"
    variant_convention: str = "pascal"
    field_prefix: str = ""
    pluralize_vectors: bool = False


@dataclass
class ReservedSection:
    words: tuple[str, ...] = ()  # the target's WHOLE reserved-word list
    members: tuple[str, ...] = ()  # member idents the target's templates reserve
    type_suffixes: tuple[str, ...] = ()  # compositions templates append to type idents
    invalid_prefix: str = "_"


@dataclass
class RenderEntry:
    """One manifest row: render `template` for every plate the row selects,
    writing to the `output` pattern (casing placeholders like {snake} come
    from the plate's name). A row selects either by `strategies` (the normal
    shape-driven case) or by `types` -- exact wire names, for bespoke
    handling of individual types. Type rows OVERRIDE strategy rows: a plate
    named by any type row is rendered only by its type rows, so custom code
    for one element or attribute is a config-and-template change, never a
    generator change. A once-per-target row has neither."""

    template: str
    output: str
    strategies: tuple[str, ...] = ()
    types: tuple[str, ...] = ()


@dataclass
class RenderSection:
    """The render manifest: which templates produce which files. Its presence
    selects the press pipeline."""

    dir: Path  # the target's templates directory, resolved
    format: tuple[str, ...] = ()  # optional post-render command; {dir} expands
    types: list[RenderEntry] = field(default_factory=list)
    once: list[RenderEntry] = field(default_factory=list)


@dataclass
class DocsSection:
    # Width of the wrapped doc TEXT, excluding comment syntax (templates add
    # their own prefixes). 97 + a 3-character prefix is the 100-column house
    # style.
    wrap: int = 97


@dataclass
class Config:
    path: Path = Path(".")  # the config file itself, resolved
    xsd: Path | None = None  # the MusicXML XSD this target generates from
    output_dir: Path | None = None  # where generated code lands, resolved
    sounds_xml: Path | None = None  # companion sounds file to fold in, or None
    target: TargetSection = field(default_factory=TargetSection)
    vars: dict[str, str] = field(default_factory=dict)  # freeform, for templates
    naming: NamingSection = field(default_factory=NamingSection)
    reserved: ReservedSection = field(default_factory=ReservedSection)
    types: dict[str, str] = field(default_factory=dict)  # primitive overrides
    docs: DocsSection = field(default_factory=DocsSection)
    renames: Renames = field(default_factory=Renames)
    # The import-policy repair table: a required
    # attribute MISSING from a document gets this default injected by the
    # parser; a missing required attribute with no entry is a parse error.
    # Keyed (complex type wire, attribute wire) -> wire literal.
    import_attribute_defaults: dict[tuple[str, str], str] = field(default_factory=dict)
    render: RenderSection | None = None  # presence selects the press pipeline


def load(config_path) -> Config:
    """Parse config.toml. Paths inside it are interpreted relative to the
    config file's own directory, so a target's config stays self-contained."""
    path = Path(config_path).resolve()
    if not path.exists():
        raise FileNotFoundError(f"config not found: {path}")
    with open(path, "rb") as f:
        data = tomllib.load(f)
    base = path.parent
    _check_keys(
        data,
        {"input", "output", "sounds", "target", "naming", "reserved", "types",
         "docs", "rename", "vars", "render", "import"},
        "top level",
    )
    _check_keys(data.get("input", {}), {"xsd"}, "input")
    _check_keys(data.get("output", {}), {"dir"}, "output")
    _check_keys(data.get("sounds", {}), {"xml"}, "sounds")

    # A shared naming base (design: [naming] extends) contributes [naming]
    # keys and [rename.*] entries; the target's own win on any conflict.
    data = _apply_extends(data, base)

    # Each target pins its own MusicXML version: the schema it generates from
    # is part of the target's identity, not a global default.
    xsd = None
    inp = data.get("input", {})
    if inp.get("xsd"):
        xsd = (base / inp["xsd"]).resolve()
        if not xsd.exists():
            raise FileNotFoundError(f"xsd not found: {xsd}")

    output_dir = None
    out = data.get("output", {})
    if out.get("dir"):
        output_dir = (base / out["dir"]).resolve()

    # Companion sounds patch is on iff [sounds] xml names a file (see
    # gen.ir.sounds). Resolve and existence-check it here so a bad path fails
    # at config load, not deep in the lowering.
    sounds_xml = None
    sounds = data.get("sounds", {})
    if sounds.get("xml"):
        sounds_xml = (base / sounds["xml"]).resolve()
        if not sounds_xml.exists():
            raise FileNotFoundError(f"sounds file not found: {sounds_xml}")

    return Config(
        path=path,
        xsd=xsd,
        output_dir=output_dir,
        sounds_xml=sounds_xml,
        target=_target(data.get("target", {})),
        vars=_vars(data.get("vars", {})),
        naming=_naming(data.get("naming", {})),
        reserved=_reserved(data.get("reserved", {})),
        types=_types(data.get("types", {})),
        docs=_docs(data.get("docs", {})),
        renames=_renames(data.get("rename", {})),
        import_attribute_defaults=_import_defaults(data.get("import", {})),
        render=_render(data["render"], base) if "render" in data else None,
    )


# --------------------------------------------------------------------------- #
# Section parsers. Each takes the raw TOML table and fails loud on unknown
# keys, so a typo is a config error, not a silently ignored line.
# --------------------------------------------------------------------------- #


def _import_defaults(t: dict) -> dict[tuple[str, str], str]:
    """[import] attribute-defaults: { "type" = { "attr" = "literal" } }."""
    _check_keys(t, {"attribute-defaults"}, "import")
    out: dict[tuple[str, str], str] = {}
    for type_wire, attrs in t.get("attribute-defaults", {}).items():
        if not isinstance(attrs, dict):
            raise ConfigError(
                f"[import.attribute-defaults] {type_wire}: expected a table "
                f"of attribute = literal"
            )
        for attr_wire, literal in attrs.items():
            if not isinstance(literal, str):
                raise ConfigError(
                    f"[import.attribute-defaults] {type_wire}.{attr_wire}: "
                    f"expected a string literal"
                )
            out[(type_wire, attr_wire)] = literal
    return out


def _check_keys(table: dict, allowed: set[str], where: str) -> None:
    unknown = set(table) - allowed
    if unknown:
        raise ConfigError(f"unknown key(s) in [{where}]: {', '.join(sorted(unknown))}")


def _target(t: dict) -> TargetSection:
    _check_keys(t, {"symbol-prefix", "inheritance", "variant-scope"}, "target")
    section = TargetSection(
        symbol_prefix=t.get("symbol-prefix", ""),
        inheritance=bool(t.get("inheritance", True)),
        variant_scope=t.get("variant-scope", "bare"),
    )
    if section.variant_scope not in ("bare", "composed"):
        raise ConfigError(
            f"[target] variant-scope = {section.variant_scope!r}: expected bare or composed"
        )
    return section


def _vars(t: dict) -> dict[str, str]:
    """Freeform key-values passed verbatim to templates ({{target.vars.x}}).
    The generator never interprets them; this is where anything that cannot
    be defined without naming a language belongs."""
    for k, v in t.items():
        if not isinstance(v, str):
            raise ConfigError(f"[vars] {k} must be a string")
    return dict(t)


def _string_list(value, where: str) -> tuple[str, ...]:
    """A TOML array of strings. A bare string is rejected rather than being
    silently exploded into characters."""
    if not isinstance(value, list) or not all(isinstance(x, str) for x in value):
        raise ConfigError(f"[{where}] must be an array of strings")
    return tuple(value)


def _naming(t: dict) -> NamingSection:
    _check_keys(
        t,
        {
            "extends", "acronyms", "type-convention", "field-convention",
            "variant-convention", "field-prefix", "pluralize-vectors",
        },
        "naming",
    )
    section = NamingSection(
        acronyms=_string_list(t["acronyms"], "naming.acronyms") if "acronyms" in t else None,
        type_convention=t.get("type-convention", "pascal"),
        field_convention=t.get("field-convention", "snake"),
        variant_convention=t.get("variant-convention", "pascal"),
        field_prefix=t.get("field-prefix", ""),
        pluralize_vectors=bool(t.get("pluralize-vectors", False)),
    )
    for key in ("type_convention", "field_convention", "variant_convention"):
        value = getattr(section, key)
        if value not in CONVENTIONS:
            raise ConfigError(
                f"[naming] {key.replace('_', '-')} = {value!r} is not a "
                f"registered convention ({', '.join(sorted(CONVENTIONS))})"
            )
    return section


def _reserved(t: dict) -> ReservedSection:
    _check_keys(t, {"words", "members", "type-suffixes", "invalid-prefix"}, "reserved")
    return ReservedSection(
        words=_string_list(t["words"], "reserved.words") if "words" in t else (),
        members=_string_list(t["members"], "reserved.members") if "members" in t else (),
        type_suffixes=_string_list(t["type-suffixes"], "reserved.type-suffixes")
        if "type-suffixes" in t
        else (),
        invalid_prefix=t.get("invalid-prefix", "_"),
    )


def _types(t: dict) -> dict[str, str]:
    for k, v in t.items():
        if not isinstance(v, str):
            raise ConfigError(f"[types] {k} must be a string target type")
    return dict(t)


def _render_entry(t: dict, where: str, once: bool) -> RenderEntry:
    allowed = {"template", "output"} | (set() if once else {"strategies", "types"})
    _check_keys(t, allowed, where)
    for key in ("template", "output"):
        if not isinstance(t.get(key), str) or not t[key]:
            raise ConfigError(f"[{where}] requires a non-empty '{key}' string")
    strategies: tuple[str, ...] = ()
    types: tuple[str, ...] = ()
    if not once:
        strategies = tuple(_string_list(t.get("strategies", []), f"{where}.strategies"))
        types = tuple(_string_list(t.get("types", []), f"{where}.types"))
        if bool(strategies) == bool(types):
            raise ConfigError(
                f"[{where}] requires exactly one of 'strategies' or 'types'"
            )
    return RenderEntry(
        template=t["template"], output=t["output"], strategies=strategies, types=types
    )


def _render(t: dict, base: Path) -> RenderSection:
    _check_keys(t, {"dir", "format", "type", "once"}, "render")
    if not isinstance(t.get("dir"), str) or not t["dir"]:
        raise ConfigError("[render] requires a 'dir' (the templates directory)")
    directory = (base / t["dir"]).resolve()
    if not directory.is_dir():
        raise FileNotFoundError(f"templates directory not found: {directory}")
    section = RenderSection(
        dir=directory,
        format=tuple(_string_list(t["format"], "render.format")) if "format" in t else (),
        types=[
            _render_entry(e, "render.type", once=False) for e in t.get("type", [])
        ],
        once=[
            _render_entry(e, "render.once", once=True) for e in t.get("once", [])
        ],
    )
    if not section.types and not section.once:
        raise ConfigError("[render] declares no template entries")
    return section


def _docs(t: dict) -> DocsSection:
    _check_keys(t, {"wrap"}, "docs")
    return DocsSection(wrap=int(t.get("wrap", 97)))


# --------------------------------------------------------------------------- #
# Renames (design 6.2/6.3): two tiers (fundamental + per-convention), four
# addressable kinds, with enum values scoped to their enum and attributes
# optionally scoped to their owner type.
# --------------------------------------------------------------------------- #


def _entry(value, where: str) -> RenameEntry:
    """A rename value is either the string shorthand (sugar for a table with
    only `fundamental`) or a table of fundamental/convention keys."""
    if isinstance(value, str):
        return RenameEntry(fundamental=value)
    if isinstance(value, dict):
        unknown = set(value) - _ENTRY_KEYS
        if unknown:
            raise ConfigError(
                f"unknown key(s) in [{where}]: {', '.join(sorted(unknown))} "
                f"(expected fundamental or a convention: {', '.join(sorted(CONVENTIONS))})"
            )
        bad = [k for k, v in value.items() if not isinstance(v, str)]
        if bad:
            raise ConfigError(f"[{where}] {bad[0]} must be a string")
        if not value:
            raise ConfigError(f"[{where}] is empty: set fundamental or a convention")
        return RenameEntry(
            fundamental=value.get("fundamental"),
            cased={k: v for k, v in value.items() if k != "fundamental"},
        )
    raise ConfigError(f"[{where}] must be a string or a table")


def _is_entry_table(value) -> bool:
    return isinstance(value, dict) and set(value) <= _ENTRY_KEYS


def _renames(t: dict) -> Renames:
    unknown = set(t) - set(_RENAME_KINDS) - {"group", "attribute-group"}
    if unknown:
        raise ConfigError(f"unknown rename kind(s): {', '.join(sorted(unknown))}")
    for reserved_kind in ("group", "attribute-group"):
        if reserved_kind in t:
            raise ConfigError(
                f"rename kind '{reserved_kind}' is reserved for targets that emit "
                f"shared fragments; no current target does"
            )

    for kind in _RENAME_KINDS:
        if kind in t and not isinstance(t[kind], dict):
            raise ConfigError(f"[rename.{kind}] must be a table")

    r = Renames()
    for wire, value in t.get("type", {}).items():
        r.types[wire] = _entry(value, f"rename.type.{wire}")
    for wire, value in t.get("element", {}).items():
        r.elements[wire] = _entry(value, f"rename.element.{wire}")

    # [rename.attribute] mixes global entries (string, or a table of entry
    # keys) with owner scopes (a table keyed by attribute names). The key sets
    # are disjoint: entry keys are fundamental/conventions, never wire names.
    for key, value in t.get("attribute", {}).items():
        if isinstance(value, str) or _is_entry_table(value):
            r.attributes[key] = _entry(value, f"rename.attribute.{key}")
        elif isinstance(value, dict):
            for attr, sub in value.items():
                r.scoped_attributes[(key, attr)] = _entry(
                    sub, f"rename.attribute.{key}.{attr}"
                )
        else:
            raise ConfigError(f"[rename.attribute] {key} must be a string or a table")

    for enum, table in t.get("enum-value", {}).items():
        if not isinstance(table, dict):
            raise ConfigError(f"[rename.enum-value.{enum}] must be a table of values")
        for wire, value in table.items():
            r.enum_values[(enum, wire)] = _entry(value, f"rename.enum-value.{enum}.{wire}")
    return r


# --------------------------------------------------------------------------- #
# Shared naming base ([naming] extends)
# --------------------------------------------------------------------------- #


def _apply_extends(data: dict, base_dir: Path) -> dict:
    """Merge a shared base file under the target's config: the base
    contributes [naming] keys and [rename] entries; the target's own win per
    key/entry. Anything else in the base is an error, as is chaining bases."""
    extends = data.get("naming", {}).get("extends")
    if not extends:
        return data
    base_path = (base_dir / extends).resolve()
    if not base_path.exists():
        raise FileNotFoundError(f"naming base not found: {base_path}")
    with open(base_path, "rb") as f:
        shared = tomllib.load(f)
    _check_keys(shared, {"naming", "rename"}, f"naming base {base_path.name}")
    if "extends" in shared.get("naming", {}):
        raise ConfigError(f"naming base {base_path.name} may not chain to another base")

    merged = dict(data)
    naming = dict(shared.get("naming", {}))
    naming.update(data.get("naming", {}))
    naming.pop("extends", None)
    merged["naming"] = naming

    rename: dict = {}
    for kind in set(shared.get("rename", {})) | set(data.get("rename", {})):
        base_table = shared.get("rename", {}).get(kind, {})
        own_table = data.get("rename", {}).get(kind, {})
        table: dict = {}
        for key in list(base_table) + [k for k in own_table if k not in base_table]:
            b, o = base_table.get(key), own_table.get(key)
            b_scope = isinstance(b, dict) and not _is_entry_table(b)
            o_scope = isinstance(o, dict) and not _is_entry_table(o)
            if b is not None and o is not None and b_scope != o_scope:
                # One side addresses a scope table, the other a single entry:
                # a silent wholesale replacement would quietly drop the
                # base's renames, so the disagreement is an error.
                raise ConfigError(
                    f"[rename.{kind}.{key}]: the target and its naming base "
                    f"disagree on whether this is a scope or an entry"
                )
            if b_scope and o_scope:
                # A nested scope (an enum's value table, an owner's attribute
                # table): merge per inner entry, target winning.
                table[key] = {**b, **o}
            else:
                table[key] = o if key in own_table else b
        rename[kind] = table
    if rename:
        merged["rename"] = rename
    return merged
