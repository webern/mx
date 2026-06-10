"""Go templates for the four complex shapes.

One template per strategy: value-class, composite-class, flag, attrs-class,
inherit. Each complex type renders as a struct plus a parse and a serialize
function against the etree DOM.

Representation decisions (these are the Go backend's spelling of the plate
facts, chosen for round-trip fidelity):

  - Every attribute is presence-tracked (a pointer field), required or not:
    the corert contract is "write back exactly what was parsed", and corpus
    files do omit required attributes.
  - A composite stores its children as ONE ordered list (`Children
    []XChild`, a kind-tagged struct with a typed pointer per possible child
    name) rather than one field per member. Interleaved choice elements
    (measure's music-data, note's grace/cue branches) round-trip in document
    order for free, which per-member vectors cannot do.
  - Parsing is strict about NAMES (an unknown attribute or child element is
    an error: the version gate keeps incompatible documents out, so an
    unknown name is a generator gap, not data) and lenient about VALUES
    (the typed Parse* functions degrade deterministically).
  - Namespace declarations (xmlns*) are skipped wherever they appear; the
    document entry point preserves the root's (see document.py).
  - A derived type embeds its base (field promotion gives the flat view) and
    parses/serializes the merged member set in one pass, appending children
    to the base chain's owner.
"""

from __future__ import annotations

from gen.emit.go.common import doc_comment, file_frame, go_string
from gen.plates.model import (
    ComplexPlate,
    Member,
    Plates,
    attribute_members as _attr_members,
    element_members as _element_members,
    value_member as _value_member,
)

# IR primitive -> (Go type, parse expr template, to-string expr template).
_PRIM = {
    "decimal": ("float64", "parseDecimal({0})", "formatDecimal({0})"),
    "integer": ("int", "parseInt({0})", "formatInt({0})"),
    "positive_integer": ("int", "parseInt({0})", "formatInt({0})"),
    "non_negative_integer": ("int", "parseInt({0})", "formatInt({0})"),
    "string": ("string", "{0}", "{0}"),
    "token": ("string", "{0}", "{0}"),
    "nmtoken": ("string", "{0}", "{0}"),
    "date": ("string", "{0}", "{0}"),
}


def _go_type(member: Member) -> str:
    if member.type_ref.category == "primitive":
        return _PRIM[member.type_ref.wire][0]
    return member.type_ref.ident


def _parse_expr(member: Member, source: str) -> str:
    """The expression turning wire text into the member's Go value."""
    if member.type_ref.category == "primitive":
        return _PRIM[member.type_ref.wire][1].format(source)
    return f"Parse{member.type_ref.ident}({source})"


def _string_expr(member: Member, source: str) -> str:
    """The expression turning the member's Go value back into wire text."""
    if member.type_ref.category == "primitive":
        return _PRIM[member.type_ref.wire][2].format(source)
    return f"{source}.String()"


def complex_file(plates: Plates, plate: ComplexPlate) -> str:
    if plate.strategy == "inherit":
        body = _inherit_body(plates, plate)
    else:
        body = _class_body(plates, plate)
    return file_frame(plates, body, imports=["fmt", "github.com/beevik/etree"])


# --------------------------------------------------------------------------- #
# The shared pieces: struct, attribute loop, child loop
# --------------------------------------------------------------------------- #


def _struct_lines(plates: Plates, plate: ComplexPlate, members: list[Member],
                  embed: str | None, children_of: str | None) -> list[str]:
    wrap = plates.target.doc_style.wrap
    lines = doc_comment(plate.doc, wrap)
    lines += [f"type {plate.ident} struct {{"]
    if embed:
        lines += [f"\t{embed}"]
    for m in _attr_members(members):
        lines += [f"\t{m.ident} *{_go_type(m)} // attribute {go_string(m.name.wire)}"]
    value = _value_member(members)
    if value is not None and not embed:
        lines += [f"\t{value.ident} {_go_type(value)} // text content"]
    if children_of == plate.ident:
        lines += [f"\tChildren []{plate.ident}Child // child elements in document order"]
    lines += ["}", ""]
    return lines


def _child_struct_lines(plates: Plates, plate: ComplexPlate) -> list[str]:
    elements = _element_members(plate.members)
    ident = plate.ident
    lines = [
        f"// {ident}Child is one child element of {ident}: exactly one field",
        "// is non-nil, and that pointer says which element this is. (No kind",
        "// discriminator: schema element names like harmony's <kind> would",
        "// collide with a synthetic field.) Constructing a child with zero or",
        "// multiple fields set is undefined: serialization writes the first",
        "// non-nil field in schema order and nothing when all are nil.",
        f"type {ident}Child struct {{",
    ]
    for m in elements:
        lines += [f"\t{m.ident} *{_go_type(m)}"]
    lines += ["}", ""]
    return lines


def _attr_loop(plates: Plates, members: list[Member]) -> list[str]:
    lines = [
        "\tfor _, a := range el.Attr {",
        '\t\tif a.Space == "xmlns" || (a.Space == "" && a.Key == "xmlns") {',
        "\t\t\tcontinue",
        "\t\t}",
        "\t\tswitch a.FullKey() {",
    ]
    for m in _attr_members(members):
        lines += [f"\t\tcase {go_string(m.name.wire)}:"]
        lines += [f"\t\t\tv := {_parse_expr(m, 'a.Value')}"]
        lines += [f"\t\t\tm.{m.ident} = &v"]
    lines += [
        "\t\tdefault:",
        '\t\t\treturn nil, fmt.Errorf("unknown attribute %q on <%s>", a.FullKey(), el.Tag)',
        "\t\t}",
        "\t}",
    ]
    return lines


def _attr_writes(plates: Plates, members: list[Member]) -> list[str]:
    lines = []
    for m in _attr_members(members):
        lines += [
            f"\tif m.{m.ident} != nil {{",
            f"\t\tel.CreateAttr({go_string(m.name.wire)}, {_string_expr(m, f'(*m.{m.ident})')})",
            "\t}",
        ]
    return lines


def _child_parse_loop(plates: Plates, owner: ComplexPlate) -> list[str]:
    """The children dispatch: element name -> typed parse, appended in
    document order. `owner` is the plate whose Child struct holds them."""
    ident = owner.ident
    lines = ["\tfor _, c := range el.ChildElements() {", "\t\tswitch c.Tag {"]
    for m in _element_members(owner.members):
        lines += [f"\t\tcase {go_string(m.name.wire)}:"]
        if m.type_ref.category == "complex":
            lines += [
                f"\t\t\tv, err := parse{m.type_ref.ident}(c)",
                "\t\t\tif err != nil {",
                "\t\t\t\treturn nil, err",
                "\t\t\t}",
                f"\t\t\tm.Children = append(m.Children, {ident}Child{{{m.ident}: v}})",
            ]
        else:
            lines += [f"\t\t\tv := {_parse_expr(m, 'c.Text()')}"]
            lines += [
                f"\t\t\tm.Children = append(m.Children, {ident}Child{{{m.ident}: &v}})",
            ]
    lines += [
        "\t\tdefault:",
        '\t\t\treturn nil, fmt.Errorf("unknown element <%s> in <%s>", c.Tag, el.Tag)',
        "\t\t}",
        "\t}",
    ]
    return lines


def _child_serialize_loop(plates: Plates, owner: ComplexPlate) -> list[str]:
    lines = ["\tfor _, ch := range m.Children {", "\t\tswitch {"]
    for m in _element_members(owner.members):
        lines += [f"\t\tcase ch.{m.ident} != nil:"]
        if m.type_ref.category == "complex":
            lines += [f"\t\t\tserialize{m.type_ref.ident}(ch.{m.ident}, el, {go_string(m.name.wire)})"]
        else:
            lines += [
                f"\t\t\tel.CreateElement({go_string(m.name.wire)}).SetText({_string_expr(m, f'(*ch.{m.ident})')})",
            ]
    lines += ["\t\t}", "\t}"]
    return lines


def _reject_children() -> list[str]:
    return [
        "\tfor _, c := range el.ChildElements() {",
        '\t\treturn nil, fmt.Errorf("unknown element <%s> in <%s>", c.Tag, el.Tag)',
        "\t}",
    ]


# --------------------------------------------------------------------------- #
# value-class | flag | attrs-class | composite-class
# --------------------------------------------------------------------------- #


def _class_body(plates: Plates, plate: ComplexPlate) -> list[str]:
    ident = plate.ident
    has_children = plate.strategy == "composite-class" and _element_members(plate.members)
    value = _value_member(plate.members)

    lines = _struct_lines(
        plates, plate, plate.members,
        embed=None,
        children_of=ident if has_children else None,
    )
    if has_children:
        lines += _child_struct_lines(plates, plate)

    lines += [f"func parse{ident}(el *etree.Element) (*{ident}, error) {{"]
    lines += [f"\tm := &{ident}{{}}"]
    lines += _attr_loop(plates, plate.members)
    if value is not None:
        lines += [f"\tm.{value.ident} = {_parse_expr(value, 'el.Text()')}"]
    if has_children:
        lines += _child_parse_loop(plates, plate)
    else:
        lines += _reject_children()
    lines += ["\treturn m, nil", "}", ""]

    lines += [f"func serialize{ident}(m *{ident}, parent *etree.Element, tag string) {{"]
    inert = not _attr_members(plate.members) and value is None and not has_children
    if inert:
        # Presence is the only information this element carries.
        lines += ["\t_ = m", "\tparent.CreateElement(tag)"]
    else:
        lines += ["\tel := parent.CreateElement(tag)"]
        if value is not None:
            lines += [f"\tel.SetText({_string_expr(value, f'm.{value.ident}')})"]
        lines += _attr_writes(plates, plate.members)
        if has_children:
            lines += _child_serialize_loop(plates, plate)
    lines += ["}"]
    return lines


# --------------------------------------------------------------------------- #
# inherit (derived): embed the base, parse/serialize the merged view
# --------------------------------------------------------------------------- #


def _inherit_body(plates: Plates, plate: ComplexPlate) -> list[str]:
    ident = plate.ident
    members = plate.all_members or plate.members
    owner = plates.children_owner(plate)

    lines = _struct_lines(
        plates, plate, plate.members,  # own attrs only: the base is embedded
        embed=plate.base.ident,
        children_of=None,
    )

    lines += [f"func parse{ident}(el *etree.Element) (*{ident}, error) {{"]
    lines += [f"\tm := &{ident}{{}}"]
    # The merged attribute set parses in one pass; field promotion routes
    # base attributes through the embedded struct.
    lines += _attr_loop(plates, members)
    value = _value_member(members)
    if value is not None:
        lines += [f"\tm.{value.ident} = {_parse_expr(value, 'el.Text()')}"]
    if owner is not None:
        lines += _child_parse_loop(plates, owner)
    else:
        lines += _reject_children()
    lines += ["\treturn m, nil", "}", ""]

    lines += [f"func serialize{ident}(m *{ident}, parent *etree.Element, tag string) {{"]
    lines += ["\tel := parent.CreateElement(tag)"]
    if value is not None:
        lines += [f"\tel.SetText({_string_expr(value, f'm.{value.ident}')})"]
    lines += _attr_writes(plates, members)
    if owner is not None:
        lines += _child_serialize_loop(plates, owner)
    lines += ["}"]
    return lines
