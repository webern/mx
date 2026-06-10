#include "compare.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int is_equivalent(const char *a, const char *b) {
    if (!a) a = "";
    if (!b) b = "";
    if (strcmp(a, b) == 0) return 1;

    char *ea, *eb;
    long long la = strtoll(a, &ea, 10);
    long long lb = strtoll(b, &eb, 10);
    if (*ea == '\0' && *eb == '\0' && ea != a && eb != b && la == lb)
        return 1;

    double da = strtod(a, &ea);
    double db = strtod(b, &eb);
    if (*ea == '\0' && *eb == '\0' && ea != a && eb != b &&
        !isnan(da) && !isnan(db) && !isinf(da) && !isinf(db) &&
        fabs(da - db) < 0.00000001)
        return 1;

    return 0;
}

/* The element's DIRECT text content only (not the subtree concatenation
   xmlNodeGetContent computes): subtree text would re-compare every leaf at
   every ancestor, and a numerically-equivalent leaf reformat ("1.0" -> "1")
   would fail the ancestors' exact comparison. */
static char *node_text(xmlNodePtr node) {
    size_t len = 0;
    for (xmlNodePtr c = node->children; c; c = c->next)
        if (c->type == XML_TEXT_NODE || c->type == XML_CDATA_SECTION_NODE)
            len += c->content ? strlen((const char *)c->content) : 0;
    char *s = malloc(len + 1);
    if (!s) abort();
    s[0] = '\0';
    for (xmlNodePtr c = node->children; c; c = c->next)
        if (c->type == XML_TEXT_NODE || c->type == XML_CDATA_SECTION_NODE)
            if (c->content)
                strcat(s, (const char *)c->content);
    return s;
}

/* The attribute's qualified name: a parsed document carries xlink:href as
   (ns prefix "xlink", name "href") while a serialized one may carry the
   literal name "xlink:href"; both must compare equal. */
static void attr_qname(xmlAttrPtr a, char *buf, size_t cap) {
    int n;
    if (a->ns && a->ns->prefix)
        n = snprintf(buf, cap, "%s:%s", (const char *)a->ns->prefix,
                     (const char *)a->name);
    else
        n = snprintf(buf, cap, "%s", (const char *)a->name);
    if (n < 0 || (size_t)n >= cap)
        abort(); /* a truncated name must never compare equal to another */
}

/* Namespace declarations live in nsDef, never in the attribute list, so
   they need their own comparison or the model's namespace preservation
   would be unverifiable. Counts each (prefix, href) pair of `a` found
   exactly once in `b`. */
static int ns_decls_match(xmlNodePtr a, xmlNodePtr b) {
    int na = 0, nb = 0;
    for (xmlNsPtr ns = a->nsDef; ns; ns = ns->next)
        na++;
    for (xmlNsPtr ns = b->nsDef; ns; ns = ns->next)
        nb++;
    if (na != nb)
        return 0;
    for (xmlNsPtr ns = a->nsDef; ns; ns = ns->next) {
        int found = 0;
        for (xmlNsPtr other = b->nsDef; other; other = other->next) {
            int prefix_eq =
                (!ns->prefix && !other->prefix) ||
                (ns->prefix && other->prefix &&
                 strcmp((const char *)ns->prefix, (const char *)other->prefix) == 0);
            if (prefix_eq &&
                strcmp((const char *)ns->href, (const char *)other->href) == 0) {
                found = 1;
                break;
            }
        }
        if (!found)
            return 0;
    }
    return 1;
}

static CompareResult do_compare(xmlNodePtr expected, xmlNodePtr actual,
                                 const char *path) {
    CompareResult r = {0, ""};

    if (!expected || !actual) {
        r.failed = 1;
        snprintf(r.detail, sizeof(r.detail), "nil element at %s", path);
        return r;
    }

    if (strcmp((const char *)expected->name, (const char *)actual->name) != 0) {
        r.failed = 1;
        snprintf(r.detail, sizeof(r.detail),
                 "element name mismatch at %s: expected '%s', actual '%s'",
                 path, expected->name, actual->name);
        return r;
    }

    if (!ns_decls_match(expected, actual)) {
        r.failed = 1;
        snprintf(r.detail, sizeof(r.detail),
                 "namespace declaration mismatch at %s", path);
        return r;
    }

    char *et = node_text(expected);
    char *at = node_text(actual);
    if (!is_equivalent(et, at)) {
        r.failed = 1;
        snprintf(r.detail, sizeof(r.detail),
                 "mismatch at %s: expected '%s', actual '%s'", path, et, at);
        free(et);
        free(at);
        return r;
    }
    free(et);
    free(at);

    int ea_count = 0, aa_count = 0;
    for (xmlAttrPtr a = expected->properties; a; a = a->next) ea_count++;
    for (xmlAttrPtr a = actual->properties; a; a = a->next) aa_count++;
    if (ea_count != aa_count) {
        r.failed = 1;
        snprintf(r.detail, sizeof(r.detail),
                 "attribute count mismatch at %s", path);
        return r;
    }

    xmlAttrPtr ea = expected->properties;
    xmlAttrPtr aa = actual->properties;
    while (ea && aa) {
        char ename[128], aname[128];
        attr_qname(ea, ename, sizeof(ename));
        attr_qname(aa, aname, sizeof(aname));
        xmlChar *ev = xmlNodeListGetString(expected->doc, ea->children, 1);
        xmlChar *av = xmlNodeListGetString(actual->doc, aa->children, 1);
        int name_eq = strcmp(ename, aname) == 0;
        int val_eq = is_equivalent((const char *)ev, (const char *)av);
        if (!name_eq || !val_eq) {
            r.failed = 1;
            snprintf(r.detail, sizeof(r.detail),
                     "attribute mismatch at %s[@%s]", path, ename);
            xmlFree(ev);
            xmlFree(av);
            return r;
        }
        xmlFree(ev);
        xmlFree(av);
        ea = ea->next;
        aa = aa->next;
    }

    xmlNodePtr ec = expected->children;
    xmlNodePtr ac = actual->children;
    int idx = 0;
    while (ec || ac) {
        while (ec && ec->type != XML_ELEMENT_NODE) ec = ec->next;
        while (ac && ac->type != XML_ELEMENT_NODE) ac = ac->next;
        if (!ec && !ac) break;
        if (!ec || !ac) {
            r.failed = 1;
            snprintf(r.detail, sizeof(r.detail),
                     "child count mismatch at %s", path);
            return r;
        }
        char child_path[2048];
        snprintf(child_path, sizeof(child_path), "%s/%s[%d]",
                 path, ec->name, idx);
        r = do_compare(ec, ac, child_path);
        if (r.failed) return r;
        ec = ec->next;
        ac = ac->next;
        idx++;
    }

    return r;
}

CompareResult compare_elements(xmlNodePtr expected, xmlNodePtr actual) {
    const char *root_name = expected ? (const char *)expected->name : "/";
    return do_compare(expected, actual, root_name);
}
