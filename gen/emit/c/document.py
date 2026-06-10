"""The C document entry points: MxDocument, from_xdoc, to_xdoc, free.

Generated from the plates' roots. The document also preserves the root
element's namespace declarations (libxml2 keeps them in nsDef, never in the
attribute list, so the per-type parsers never see them)."""

from __future__ import annotations

from gen.emit.c.common import c_string, fn_name, fn_prefix, header_file, impl_file
from gen.plates.model import Plates


def document_stem(plates: Plates) -> str:
    return plates.target.file_prefix + "document"


def document_files(plates: Plates, stem: str, rt: str):
    p = fn_prefix(plates)
    type_prefix = plates.target.prefix
    ns_type = f"{type_prefix}Namespace"
    doc_type = f"{type_prefix}Document"
    roots = [(r, plates.plate(r.wire)) for r in plates.roots]

    decl = [
        "/* A namespace declaration preserved from the document root. */",
        "typedef struct {",
        "    char *prefix; /* NULL for the default namespace */",
        "    char *href;",
        f"}} {ns_type};",
        "",
        "/* A parsed MusicXML document: exactly one root is set. */",
        "typedef struct {",
    ]
    for ref, plate in roots:
        decl += [f"    {plate.ident} *{plate.name.snake};"]
    decl += [
        f"    {ns_type} *namespaces;",
        "    size_t namespaces_count;",
        f"}} {doc_type};",
        "",
        f"/* NULL on error; the message is in {p}error(). */",
        f"{doc_type} *{p}document_from_xdoc(xmlDocPtr doc);",
        "/* 0 on success; the caller owns *out. */",
        f"int {p}document_to_xdoc(const {doc_type} *d, xmlDocPtr *out);",
        f"void {p}document_free({doc_type} *d);",
    ]

    body = [
        f"{doc_type} *{p}document_from_xdoc(xmlDocPtr doc) {{",
        "    xmlNodePtr root = xmlDocGetRootElement(doc);",
        "    if (!root) {",
        f'        {p}error_set("document has no root element");',
        "        return NULL;",
        "    }",
        f"    {doc_type} *d = calloc(1, sizeof(*d));",
        "    if (!d)",
        "        abort();",
        "    size_t nscount = 0;",
        "    for (xmlNsPtr ns = root->nsDef; ns; ns = ns->next)",
        "        nscount++;",
        "    if (nscount) {",
        "        d->namespaces = calloc(nscount, sizeof(*d->namespaces));",
        "        if (!d->namespaces)",
        "            abort();",
        "        for (xmlNsPtr ns = root->nsDef; ns; ns = ns->next) {",
        f"            {ns_type} *slot = &d->namespaces[d->namespaces_count++];",
        f"            slot->prefix = ns->prefix ? {p}strdup((const char *)ns->prefix) : NULL;",
        f"            slot->href = {p}strdup((const char *)ns->href);",
        "        }",
        "    }",
        "    const char *tag = (const char *)root->name;",
    ]
    first = True
    for ref, plate in roots:
        kw = "if" if first else "} else if"
        first = False
        parse = fn_name(plates, plate.name, "parse")
        body += [
            f"    {kw} (strcmp(tag, {c_string(ref.wire)}) == 0) {{",
            f"        d->{plate.name.snake} = {parse}(root);",
            f"        if (!d->{plate.name.snake}) {{",
            f"            {p}document_free(d);",
            "            return NULL;",
            "        }",
        ]
    body += [
        "    } else {",
        f'        {p}error_set("unknown root element <%s>", tag);',
        f"        {p}document_free(d);",
        "        return NULL;",
        "    }",
        "    return d;",
        "}",
        "",
        f"int {p}document_to_xdoc(const {doc_type} *d, xmlDocPtr *out) {{",
        "    *out = NULL;",
        '    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");',
        "    if (!doc)",
        "        abort(); /* OOM policy: abort, matching the runtime's allocators */",
        "    /* Serialize under a scratch parent attached to the document so",
        "       reserved-prefix attributes (xml:lang, xml:space) resolve through",
        "       the document's implicit xml namespace. Building detached would",
        "       make libxml2 fabricate an xmlns:xml declaration on the element",
        "       that carries them, which the input never had. */",
        '    xmlNodePtr scratch = xmlNewDocNode(doc, NULL, BAD_CAST "scratch", NULL);',
        "    if (!scratch)",
        "        abort();",
        "    xmlNodePtr root = NULL;",
    ]
    first = True
    for ref, plate in roots:
        kw = "if" if first else "else if"
        first = False
        serialize = fn_name(plates, plate.name, "serialize")
        body += [
            f"    {kw} (d->{plate.name.snake})",
            f"        root = {serialize}(d->{plate.name.snake}, scratch, {c_string(ref.wire)});",
        ]
    body += [
        "    if (!root) {",
        f'        {p}error_set("document has no root");',
        "        xmlFreeNode(scratch);",
        "        xmlFreeDoc(doc);",
        "        return -1;",
        "    }",
        "    xmlUnlinkNode(root);",
        "    xmlDocSetRootElement(doc, root);",
        "    xmlFreeNode(scratch);",
        "    for (size_t i = 0; i < d->namespaces_count; i++)",
        "        xmlNewNs(root, BAD_CAST d->namespaces[i].href,",
        "                 BAD_CAST d->namespaces[i].prefix);",
        "    *out = doc;",
        "    return 0;",
        "}",
        "",
        f"void {p}document_free({doc_type} *d) {{",
        "    if (!d)",
        "        return;",
    ]
    for ref, plate in roots:
        free = fn_name(plates, plate.name, "free")
        body += [f"    {free}(d->{plate.name.snake});"]
    body += [
        "    for (size_t i = 0; i < d->namespaces_count; i++) {",
        "        free(d->namespaces[i].prefix);",
        "        free(d->namespaces[i].href);",
        "    }",
        "    free(d->namespaces);",
        "    free(d);",
        "}",
    ]

    includes = ["<libxml/tree.h>", "<stddef.h>"]
    includes += sorted({f'"{plate.file}.h"' for _, plate in roots})
    return (
        header_file(plates, stem, decl, includes),
        impl_file(plates, stem, body, [f'"{rt}.h"', "<stdlib.h>", "<string.h>"]),
    )
