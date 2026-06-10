"""C templates for the four complex shapes.

The C spelling of the same representation the Go backend uses (see
gen/emit/go/complexes.py for the rationale): presence-tracked attributes
(`bool has_x` + value), children as ONE ordered array of structs whose
typed pointers discriminate by non-NULL (exactly one set per child), strict
about names, lenient about values. C has no inheritance, so a derived type
flattens its base chain (the plates' all_members view) into a self-contained
struct.

Each type renders as a header/impl pair against the libxml2 DOM:

    MxT *mx_<t>_parse(xmlNodePtr el);              NULL on error; the message
                                                   is in mx_error()
    xmlNodePtr mx_<t>_serialize(const MxT *m, xmlNodePtr parent,
                                const char *tag);  parent NULL -> free node
    void mx_<t>_free(MxT *m);                      deep free, including m

Namespace declarations never appear in libxml2's attribute list (they live
in nsDef), so the attribute loops need no xmlns skipping; prefixed
attributes (xml:lang, xlink:href) match their qualified names.
"""

from __future__ import annotations

from gen.emit.c.api import CValue, value_api
from gen.emit.c.common import c_string, doc_comment, fn_name, fn_prefix, header_file, impl_file
from gen.plates.model import (
    ComplexPlate,
    Member,
    Plates,
    attribute_members as _attr_members,
    element_members as _element_members,
    value_member as _value_member,
)


def _child_field(plates: Plates, member: Member) -> tuple[str, bool]:
    """(field declaration type, value-is-the-pointer). Children discriminate
    by non-NULL, so every field is a pointer: complex types and boxed values
    point at their structs; char*-family values (raw string primitives AND
    string-plate typedefs) ARE their pointer and are stored unboxed."""
    if member.type_ref.category == "complex":
        return f"{member.type_ref.ident} *", False
    api = value_api(plates, member.type_ref)
    if api.is_pointer_value:
        sep = "" if api.c_type.endswith("*") else " "
        return f"{api.c_type}{sep}", True
    return f"{api.c_type} *", False


def complex_files(plates: Plates, plate: ComplexPlate, includes: list[str], rt: str):
    members = plate.members_view()
    attrs = _attr_members(members)
    elements = _element_members(members)
    value = _value_member(members)
    ident = plate.ident
    p = fn_prefix(plates)
    parse = fn_name(plates, plate.name, "parse")
    serialize = fn_name(plates, plate.name, "serialize")
    free_fn = fn_name(plates, plate.name, "free")
    wrap = plates.target.doc_style.wrap

    # ----- header ---------------------------------------------------------- #

    decl = doc_comment(plate.doc, wrap)
    if elements:
        decl += [
            f"/* One child element of {ident}: exactly one field is non-NULL,",
            "   and that pointer says which element this is. Document order is",
            "   the array order on the owning struct. Zero or multiple fields",
            "   set is undefined: serialization writes the first non-NULL field",
            "   in schema order and nothing when all are NULL. */",
            "typedef struct {",
        ]
        for m in elements:
            ctype, _ = _child_field(plates, m)
            sep = "" if ctype.endswith("*") else " "
            decl += [f"    {ctype}{sep}{m.ident};"]
        decl += [f"}} {ident}Child;", ""]

    decl += ["typedef struct {"]
    for m in attrs:
        api = value_api(plates, m.type_ref)
        sep = "" if api.c_type.endswith("*") else " "
        decl += [
            f"    bool has_{m.ident};",
            f"    {api.c_type}{sep}{m.ident}; /* attribute {m.name.wire} */",
        ]
    if value is not None:
        api = value_api(plates, value.type_ref)
        sep = "" if api.c_type.endswith("*") else " "
        decl += [f"    {api.c_type}{sep}{value.ident}; /* text content */"]
    if elements:
        decl += [
            f"    {ident}Child *children; /* child elements in document order */",
            "    size_t children_count;",
        ]
    if not attrs and value is None and not elements:
        decl += ["    char unused; /* presence is the only information */"]
    decl += [f"}} {ident};", ""]
    decl += [
        f"/* NULL on error; the message is in {p}error(). */",
        f"{ident} *{parse}(xmlNodePtr el);",
        "/* Appends under parent, or creates a free node when parent is NULL. */",
        f"xmlNodePtr {serialize}(const {ident} *m, xmlNodePtr parent, const char *tag);",
        f"void {free_fn}({ident} *m);",
    ]

    # ----- impl: parse ------------------------------------------------------ #

    body = [
        f"{ident} *{parse}(xmlNodePtr el) {{",
        f"    {ident} *m = calloc(1, sizeof(*m));",
        "    if (!m)",
        "        abort();",
        "    for (xmlAttrPtr a = el->properties; a; a = a->next) {",
        "        char aname[128];",
        "        if (a->ns && a->ns->prefix)",
        '            snprintf(aname, sizeof(aname), "%s:%s",',
        "                     (const char *)a->ns->prefix, (const char *)a->name);",
        "        else",
        '            snprintf(aname, sizeof(aname), "%s", (const char *)a->name);',
        "        xmlChar *avalue = xmlNodeListGetString(el->doc, a->children, 1);",
        '        const char *s = avalue ? (const char *)avalue : "";',
        "        (void)s;",
    ]
    first = True
    for m in attrs:
        api = value_api(plates, m.type_ref)
        kw = "if" if first else "} else if"
        first = False
        body += [
            f"        {kw} (strcmp(aname, {c_string(m.name.wire)}) == 0) {{",
            f"            m->has_{m.ident} = true;",
            f"            m->{m.ident} = {api.parse.format('s')};",
        ]
    closer = "} else {" if attrs else "{"
    body += [
        f"        {closer}",
        f'            {p}error_set("unknown attribute \\"%s\\" on <%s>", aname,',
        "                         (const char *)el->name);",
        "            xmlFree(avalue);",
        f"            {free_fn}(m);",
        "            return NULL;",
        "        }",
        "        xmlFree(avalue);",
        "    }",
    ]

    if value is not None:
        api = value_api(plates, value.type_ref)
        body += [
            "    {",
            "        xmlChar *text = xmlNodeGetContent(el);",
            '        const char *s = text ? (const char *)text : "";',
            f"        m->{value.ident} = {api.parse.format('s')};",
            "        xmlFree(text);",
            "    }",
        ]

    if elements:
        body += [
            "    size_t n = 0;",
            "    for (xmlNodePtr c = el->children; c; c = c->next)",
            "        if (c->type == XML_ELEMENT_NODE)",
            "            n++;",
            "    if (n) {",
            "        m->children = calloc(n, sizeof(*m->children));",
            "        if (!m->children)",
            "            abort();",
            "    }",
        ]
    body += [
        "    for (xmlNodePtr c = el->children; c; c = c->next) {",
        "        if (c->type != XML_ELEMENT_NODE)",
        "            continue;",
        "        const char *tag = (const char *)c->name;",
    ]
    if elements:
        body += [f"        {ident}Child *ch = &m->children[m->children_count];"]
    first = True
    for m in elements:
        kw = "if" if first else "} else if"
        first = False
        body += [f"        {kw} (strcmp(tag, {c_string(m.name.wire)}) == 0) {{"]
        if m.type_ref.category == "complex":
            child_parse = fn_name(plates, plates.plate(m.type_ref.wire).name, "parse")
            body += [
                f"            ch->{m.ident} = {child_parse}(c);",
                f"            if (!ch->{m.ident}) {{",
                f"                {free_fn}(m);",
                "                return NULL;",
                "            }",
            ]
        else:
            api = value_api(plates, m.type_ref)
            _, direct = _child_field(plates, m)
            body += [
                "            xmlChar *text = xmlNodeGetContent(c);",
                '            const char *s = text ? (const char *)text : "";',
            ]
            if direct:
                body += [f"            ch->{m.ident} = {api.parse.format('s')};"]
            else:
                body += [
                    f"            ch->{m.ident} = malloc(sizeof(*ch->{m.ident}));",
                    f"            if (!ch->{m.ident})",
                    "                abort();",
                    f"            *ch->{m.ident} = {api.parse.format('s')};",
                ]
            body += ["            xmlFree(text);"]
    closer = "} else {" if elements else "{"
    body += [
        f"        {closer}",
        f'            {p}error_set("unknown element <%s> in <%s>", tag,',
        "                         (const char *)el->name);",
        f"            {free_fn}(m);",
        "            return NULL;",
        "        }",
    ]
    if elements:
        body += ["        m->children_count++;"]
    body += ["    }", "    return m;", "}", ""]

    # ----- impl: serialize -------------------------------------------------- #

    body += [
        f"xmlNodePtr {serialize}(const {ident} *m, xmlNodePtr parent, const char *tag) {{",
        "    xmlNodePtr el = parent ? xmlNewChild(parent, NULL, BAD_CAST tag, NULL)",
        "                           : xmlNewNode(NULL, BAD_CAST tag);",
    "    if (!el)",
    "        abort(); /* OOM policy: abort, matching the runtime's allocators */",
    ]
    if value is not None:
        api = value_api(plates, value.type_ref)
        expr = api.to_string.format(f"m->{value.ident}")
        if api.to_string_owned:
            body += [
                "    {",
                f"        char *s = {expr};",
                "        xmlAddChild(el, xmlNewText(BAD_CAST s));",
                "        free(s);",
                "    }",
            ]
        else:
            body += [f"    xmlAddChild(el, xmlNewText(BAD_CAST {expr}));"]
    for m in attrs:
        api = value_api(plates, m.type_ref)
        expr = api.to_string.format(f"m->{m.ident}")
        body += [f"    if (m->has_{m.ident}) {{"]
        if api.to_string_owned:
            body += [
                f"        char *s = {expr};",
                f"        xmlSetProp(el, BAD_CAST {c_string(m.name.wire)}, BAD_CAST s);",
                "        free(s);",
            ]
        else:
            body += [f"        xmlSetProp(el, BAD_CAST {c_string(m.name.wire)}, BAD_CAST {expr});"]
        body += ["    }"]
    if elements:
        body += [
            "    for (size_t i = 0; i < m->children_count; i++) {",
            f"        const {ident}Child *ch = &m->children[i];",
        ]
        first = True
        for m in elements:
            kw = "if" if first else "} else if"
            first = False
            body += [f"        {kw} (ch->{m.ident}) {{"]
            if m.type_ref.category == "complex":
                child_ser = fn_name(plates, plates.plate(m.type_ref.wire).name, "serialize")
                body += [f"            {child_ser}(ch->{m.ident}, el, {c_string(m.name.wire)});"]
            else:
                api = value_api(plates, m.type_ref)
                _, direct = _child_field(plates, m)
                source = f"ch->{m.ident}" if direct else f"(*ch->{m.ident})"
                expr = api.to_string.format(source)
                if api.to_string_owned:
                    body += [
                        f"            char *s = {expr};",
                        f"            xmlNewTextChild(el, NULL, BAD_CAST {c_string(m.name.wire)}, BAD_CAST s);",
                        "            free(s);",
                    ]
                else:
                    body += [
                        f"            xmlNewTextChild(el, NULL, BAD_CAST {c_string(m.name.wire)}, BAD_CAST {expr});",
                    ]
        body += ["        }", "    }"]
    if not attrs and value is None and not elements:
        body += ["    (void)m;"]
    body += ["    return el;", "}", ""]

    # ----- impl: free ------------------------------------------------------- #

    body += [f"void {free_fn}({ident} *m) {{", "    if (!m)", "        return;"]
    for m in attrs:
        api = value_api(plates, m.type_ref)
        if api.free is not None:
            body += [
                f"    if (m->has_{m.ident})",
                f"        {api.free.format(f'm->{m.ident}')}",
            ]
    if value is not None:
        api = value_api(plates, value.type_ref)
        if api.free is not None:
            body += [f"    {api.free.format(f'm->{value.ident}')}"]
    if elements:
        body += [
            "    for (size_t i = 0; i < m->children_count; i++) {",
            f"        {ident}Child *ch = &m->children[i];",
        ]
        for m in elements:
            if m.type_ref.category == "complex":
                child_free = fn_name(plates, plates.plate(m.type_ref.wire).name, "free")
                body += [
                    f"        if (ch->{m.ident})",
                    f"            {child_free}(ch->{m.ident});",
                ]
            else:
                api = value_api(plates, m.type_ref)
                _, direct = _child_field(plates, m)
                if direct:
                    body += [f"        free(ch->{m.ident});"]
                elif api.free is not None:
                    body += [
                        f"        if (ch->{m.ident}) {{",
                        f"            {api.free.format(f'(*ch->{m.ident})')}",
                        f"            free(ch->{m.ident});",
                        "        }",
                    ]
                else:
                    body += [f"        free(ch->{m.ident});"]
        body += ["    }", "    free(m->children);"]
    body += ["    free(m);", "}"]

    header_includes = ["<libxml/tree.h>", "<stdbool.h>", "<stddef.h>"]
    header_includes += [f'"{inc}.h"' for inc in includes]
    return (
        header_file(plates, plate.file, decl, header_includes),
        impl_file(plates, plate.file, body,
                  [f'"{rt}.h"', "<stdlib.h>", "<string.h>"]),
    )
