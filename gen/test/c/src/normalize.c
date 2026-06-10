#include "normalize.h"

#include <ctype.h>
#include <libxml/tree.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MUSICXML_VERSION "3.0"

static const char *decimal_fields[] = {
    "top-system-distance", "dynamics",  "left-margin", "right-margin",
    "staff-distance",      "system-distance", "default-y", "default-x",
    "tenths",              "width",     NULL};

static int is_decimal_field(const char *name) {
    for (const char **f = decimal_fields; *f; f++) {
        if (strcmp(name, *f) == 0) return 1;
    }
    return 0;
}

static char *strip_trailing_zeros(const char *s) {
    if (!s || !*s) return strdup(s ? s : "");
    const char *dot = strrchr(s, '.');
    if (!dot) return strdup(s);

    size_t len = strlen(s);
    char *buf = strdup(s);
    size_t i = len - 1;
    while (i > (size_t)(dot - s) && buf[i] == '0') i--;
    if (buf[i] == '.') i--;
    buf[i + 1] = '\0';

    if (strcmp(buf, "-0") == 0) {
        buf[0] = '0';
        buf[1] = '\0';
    }
    return buf;
}

static void strip_decimal_zeros(xmlNodePtr node) {
    if (!node) return;
    if (node->type != XML_ELEMENT_NODE) return;

    int has_element_children = 0;
    for (xmlNodePtr c = node->children; c; c = c->next) {
        if (c->type == XML_ELEMENT_NODE) {
            has_element_children = 1;
            break;
        }
    }

    if (!has_element_children && is_decimal_field((const char *)node->name)) {
        xmlChar *text = xmlNodeGetContent(node);
        if (text && text[0]) {
            char *stripped = strip_trailing_zeros((const char *)text);
            xmlNodeSetContent(node, (const xmlChar *)stripped);
            free(stripped);
        }
        xmlFree(text);
    }

    for (xmlAttrPtr attr = node->properties; attr; attr = attr->next) {
        if (!is_decimal_field((const char *)attr->name)) continue;
        xmlChar *val = xmlGetProp(node, attr->name);
        if (val && val[0]) {
            char *stripped = strip_trailing_zeros((const char *)val);
            xmlSetProp(node, attr->name, (const xmlChar *)stripped);
            free(stripped);
        }
        xmlFree(val);
    }

    for (xmlNodePtr child = node->children; child; child = child->next)
        strip_decimal_zeros(child);
}

static void sort_qname(xmlAttrPtr a, char *buf, size_t cap) {
    if (a->ns && a->ns->prefix)
        snprintf(buf, cap, "%s:%s", (const char *)a->ns->prefix,
                 (const char *)a->name);
    else
        snprintf(buf, cap, "%s", (const char *)a->name);
}

static int attr_cmp(const void *a, const void *b) {
    const xmlAttrPtr *aa = a;
    const xmlAttrPtr *bb = b;
    char an[128], bn[128];
    sort_qname(*aa, an, sizeof(an));
    sort_qname(*bb, bn, sizeof(bn));
    return strcmp(an, bn);
}

static void sort_attributes(xmlNodePtr node) {
    if (!node || node->type != XML_ELEMENT_NODE) return;

    int count = 0;
    for (xmlAttrPtr a = node->properties; a; a = a->next) count++;

    if (count > 1) {
        xmlAttrPtr *arr = malloc(sizeof(xmlAttrPtr) * count);
        int i = 0;
        for (xmlAttrPtr a = node->properties; a; a = a->next)
            arr[i++] = a;
        qsort(arr, count, sizeof(xmlAttrPtr), attr_cmp);

        node->properties = arr[0];
        for (i = 0; i < count - 1; i++) {
            arr[i]->next = arr[i + 1];
            arr[i + 1]->prev = arr[i];
        }
        arr[0]->prev = NULL;
        arr[count - 1]->next = NULL;
        free(arr);
    }

    for (xmlNodePtr child = node->children; child; child = child->next)
        sort_attributes(child);
}

void set_root_version(xmlNodePtr root) {
    if (!root) return;
    xmlSetProp(root, (const xmlChar *)"version",
               (const xmlChar *)MUSICXML_VERSION);
}

/* Remove whitespace-only text nodes from every element: pretty-printing
   indentation in containers, including containers whose optional children
   are all absent. MusicXML has no mixed content, and the rule applies to
   expected and actual alike, so leaf values with real content are never
   touched and the comparison stays symmetric. */
static int is_blank_text(xmlNodePtr node) {
    if (node->type != XML_TEXT_NODE && node->type != XML_CDATA_SECTION_NODE)
        return 0;
    xmlChar *content = xmlNodeGetContent(node);
    int blank = 1;
    for (const xmlChar *p = content; p && *p; p++) {
        if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r') {
            blank = 0;
            break;
        }
    }
    xmlFree(content);
    return blank;
}

static void strip_inter_element_whitespace(xmlNodePtr node) {
    if (!node || node->type != XML_ELEMENT_NODE) return;
    xmlNodePtr child = node->children;
    while (child) {
        xmlNodePtr next = child->next;
        if (is_blank_text(child)) {
            xmlUnlinkNode(child);
            xmlFreeNode(child);
        } else if (child->type == XML_ELEMENT_NODE) {
            strip_inter_element_whitespace(child);
        }
        child = next;
    }
}

void normalize(xmlDocPtr doc) {
    if (!doc) return;
    xmlNodePtr root = xmlDocGetRootElement(doc);
    set_root_version(root);
    strip_inter_element_whitespace(root);
    strip_decimal_zeros(root);
    sort_attributes(root);
}
