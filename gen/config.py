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

from gen.plates.names import CONVENTIONS

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
    language: str = "neutral"
    namespace: str = ""
    prefix: str = ""
    inheritance: bool = True  # derived types: inherit (True) or flatten


@dataclass
class NamingSection:
    acronyms: tuple[str, ...] | None = None  # None -> the built-in default set
    type_convention: str = "pascal"
    field_convention: str = "snake"
    variant_convention: str = "pascal"
    file_convention: str = "snake"
    field_prefix: str = ""
    empty_value_word: str = "empty"
    pluralize_vectors: bool = False


@dataclass
class ReservedSection:
    words: tuple[str, ...] = ()  # extends the language defaults
    policy: str = "suffix-underscore"
    invalid_prefix: str = "_"


@dataclass
class LayoutSection:
    partition: str = "single"  # "per-type" | "single" ("grouped" reserved)
    include_style: str = "quoted"
    file_prefix: str = ""


@dataclass
class DocsSection:
    style: str | None = None  # None -> the language default
    wrap: int = 100


@dataclass
class Config:
    path: Path = Path(".")  # the config file itself, resolved
    xsd: Path | None = None  # the MusicXML XSD this target generates from
    output_dir: Path | None = None  # where generated code lands, resolved
    sounds_xml: Path | None = None  # companion sounds file to fold in, or None
    target: TargetSection = field(default_factory=TargetSection)
    naming: NamingSection = field(default_factory=NamingSection)
    reserved: ReservedSection = field(default_factory=ReservedSection)
    types: dict[str, str] = field(default_factory=dict)  # primitive overrides
    layout: LayoutSection = field(default_factory=LayoutSection)
    docs: DocsSection = field(default_factory=DocsSection)
    renames: Renames = field(default_factory=Renames)


def load(config_path) -> Config:
    """Parse config.toml. Paths inside it are interpreted relative to the
    config file's own directory, so a target's config stays self-contained."""
    path = Path(config_path).resolve()
    if not path.exists():
        raise FileNotFoundError(f"config not found: {path}")
    with open(path, "rb") as f:
        data = tomllib.load(f)
    base = path.parent

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
        naming=_naming(data.get("naming", {})),
        reserved=_reserved(data.get("reserved", {})),
        types=_types(data.get("types", {})),
        layout=_layout(data.get("layout", {})),
        docs=_docs(data.get("docs", {})),
        renames=_renames(data.get("rename", {})),
    )


# --------------------------------------------------------------------------- #
# Section parsers. Each takes the raw TOML table and fails loud on unknown
# keys, so a typo is a config error, not a silently ignored line.
# --------------------------------------------------------------------------- #


def _check_keys(table: dict, allowed: set[str], where: str) -> None:
    unknown = set(table) - allowed
    if unknown:
        raise ConfigError(f"unknown key(s) in [{where}]: {', '.join(sorted(unknown))}")


def _target(t: dict) -> TargetSection:
    _check_keys(t, {"language", "namespace", "prefix", "inheritance"}, "target")
    return TargetSection(
        language=t.get("language", "neutral"),
        namespace=t.get("namespace", ""),
        prefix=t.get("prefix", ""),
        inheritance=bool(t.get("inheritance", True)),
    )


def _naming(t: dict) -> NamingSection:
    _check_keys(
        t,
        {
            "extends", "acronyms", "type-convention", "field-convention",
            "variant-convention", "file-convention", "field-prefix",
            "empty-value-word", "pluralize-vectors",
        },
        "naming",
    )
    section = NamingSection(
        acronyms=tuple(t["acronyms"]) if "acronyms" in t else None,
        type_convention=t.get("type-convention", "pascal"),
        field_convention=t.get("field-convention", "snake"),
        variant_convention=t.get("variant-convention", "pascal"),
        file_convention=t.get("file-convention", "snake"),
        field_prefix=t.get("field-prefix", ""),
        empty_value_word=t.get("empty-value-word", "empty"),
        pluralize_vectors=bool(t.get("pluralize-vectors", False)),
    )
    for key in ("type_convention", "field_convention", "variant_convention", "file_convention"):
        value = getattr(section, key)
        if value not in CONVENTIONS:
            raise ConfigError(
                f"[naming] {key.replace('_', '-')} = {value!r} is not a "
                f"registered convention ({', '.join(sorted(CONVENTIONS))})"
            )
    return section


def _reserved(t: dict) -> ReservedSection:
    _check_keys(t, {"words", "policy", "invalid-prefix"}, "reserved")
    section = ReservedSection(
        words=tuple(t.get("words", ())),
        policy=t.get("policy", "suffix-underscore"),
        invalid_prefix=t.get("invalid-prefix", "_"),
    )
    if section.policy != "suffix-underscore":
        raise ConfigError(
            f"[reserved] policy = {section.policy!r}: only 'suffix-underscore' is implemented"
        )
    return section


def _types(t: dict) -> dict[str, str]:
    for k, v in t.items():
        if not isinstance(v, str):
            raise ConfigError(f"[types] {k} must be a string target type")
    return dict(t)


def _layout(t: dict) -> LayoutSection:
    _check_keys(t, {"partition", "include-style", "file-prefix"}, "layout")
    section = LayoutSection(
        partition=t.get("partition", "single"),
        include_style=t.get("include-style", "quoted"),
        file_prefix=t.get("file-prefix", ""),
    )
    if section.partition not in ("per-type", "single", "grouped"):
        raise ConfigError(
            f"[layout] partition = {section.partition!r}: expected per-type, single, or grouped"
        )
    return section


def _docs(t: dict) -> DocsSection:
    _check_keys(t, {"style", "wrap"}, "docs")
    return DocsSection(style=t.get("style"), wrap=int(t.get("wrap", 100)))


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
    contributes [naming] keys and whole [rename] entries; the target's own
    win per key/entry. Other sections never come from the base."""
    extends = data.get("naming", {}).get("extends")
    if not extends:
        return data
    base_path = (base_dir / extends).resolve()
    if not base_path.exists():
        raise FileNotFoundError(f"naming base not found: {base_path}")
    with open(base_path, "rb") as f:
        shared = tomllib.load(f)

    merged = dict(data)
    naming = dict(shared.get("naming", {}))
    naming.pop("extends", None)  # a base may not chain to another base
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
            if (
                isinstance(b, dict)
                and isinstance(o, dict)
                and not _is_entry_table(b)
                and not _is_entry_table(o)
            ):
                # A nested scope (an enum's value table, an owner's attribute
                # table): merge per inner entry, target winning.
                table[key] = {**b, **o}
            else:
                table[key] = o if key in own_table else b
        rename[kind] = table
    if rename:
        merged["rename"] = rename
    return merged
