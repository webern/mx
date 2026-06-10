"""The Go document entry points: Document, FromXDoc, ToXDoc.

Generated from the plates' roots. The Document also preserves the root
element's namespace declarations (a handful of corpus files declare
xmlns:xlink on the score element); the per-type parsers skip xmlns
declarations wherever they appear, and ToXDoc restores the root's.
"""

from __future__ import annotations

from gen.emit.go.common import file_frame, go_string
from gen.plates.model import Plates


def document_file(plates: Plates) -> str:
    lines = [
        "// ExtraAttr preserves an attribute outside the schema (namespace",
        "// declarations on the document root).",
        "type ExtraAttr struct {",
        "\tKey   string",
        "\tValue string",
        "}",
        "",
        "// Document is a parsed MusicXML document: exactly one root is set.",
        "type Document struct {",
    ]
    for root in plates.roots:
        lines += [f"\t{root.ident} *{root.ident}"]
    lines += ["\tRootNamespaces []ExtraAttr", "}", ""]

    lines += [
        "// FromXDoc parses an etree document into the typed MusicXML model.",
        "func FromXDoc(doc *etree.Document) (*Document, error) {",
        "\troot := doc.Root()",
        "\tif root == nil {",
        '\t\treturn nil, fmt.Errorf("document has no root element")',
        "\t}",
        "\td := &Document{}",
        "\tfor _, a := range root.Attr {",
        '\t\tif a.Space == "xmlns" || (a.Space == "" && a.Key == "xmlns") {',
        "\t\t\td.RootNamespaces = append(d.RootNamespaces, ExtraAttr{a.FullKey(), a.Value})",
        "\t\t}",
        "\t}",
        "\tswitch root.Tag {",
    ]
    for root in plates.roots:
        lines += [
            f"\tcase {go_string(root.wire)}:",
            f"\t\tv, err := parse{root.ident}(root)",
            "\t\tif err != nil {",
            "\t\t\treturn nil, err",
            "\t\t}",
            f"\t\td.{root.ident} = v",
        ]
    lines += [
        "\tdefault:",
        '\t\treturn nil, fmt.Errorf("unknown root element <%s>", root.Tag)',
        "\t}",
        "\treturn d, nil",
        "}",
        "",
        "// ToXDoc serializes the typed model back to an etree document.",
        "func ToXDoc(d *Document) (*etree.Document, error) {",
        "\tdoc := etree.NewDocument()",
        "\tswitch {",
    ]
    for root in plates.roots:
        lines += [
            f"\tcase d.{root.ident} != nil:",
            f"\t\tserialize{root.ident}(d.{root.ident}, &doc.Element, {go_string(root.wire)})",
        ]
    lines += [
        "\tdefault:",
        '\t\treturn nil, fmt.Errorf("document has no root")',
        "\t}",
        "\tfor _, a := range d.RootNamespaces {",
        "\t\tdoc.Root().CreateAttr(a.Key, a.Value)",
        "\t}",
        "\treturn doc, nil",
        "}",
    ]
    return file_frame(plates, lines, imports=["fmt", "github.com/beevik/etree"])
