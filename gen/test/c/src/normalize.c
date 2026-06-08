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

static int attr_cmp(const void *a, const void *b) {
    const xmlAttrPtr *aa = a;
    const xmlAttrPtr *bb = b;
    return strcmp((const char *)(*aa)->name, (const char *)(*bb)->name);
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

static void set_root_version(xmlNodePtr root) {
    if (!root) return;
    xmlSetProp(root, (const xmlChar *)"version",
               (const xmlChar *)MUSICXML_VERSION);
}

void normalize(xmlDocPtr doc) {
    if (!doc) return;
    xmlNodePtr root = xmlDocGetRootElement(doc);
    set_root_version(root);
    strip_decimal_zeros(root);
    sort_attributes(root);
}
