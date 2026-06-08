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

static char *node_text(xmlNodePtr node) {
    xmlChar *c = xmlNodeGetContent(node);
    if (!c) return strdup("");
    char *s = strdup((const char *)c);
    xmlFree(c);
    return s;
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
        xmlChar *ev = xmlGetProp(expected, ea->name);
        xmlChar *av = xmlGetProp(actual, aa->name);
        int name_eq = strcmp((const char *)ea->name, (const char *)aa->name) == 0;
        int val_eq = is_equivalent((const char *)ev, (const char *)av);
        if (!name_eq || !val_eq) {
            r.failed = 1;
            snprintf(r.detail, sizeof(r.detail),
                     "attribute mismatch at %s[@%s]", path, ea->name);
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
