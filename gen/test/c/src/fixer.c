#include "fixer.h"

#include <libxml/parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *fixup_sidecar_path(const char *input_path) {
    const char *dot = strrchr(input_path, '.');
    if (!dot) return NULL;
    size_t base_len = dot - input_path;
    const char *suffix = ".fixup.xml";
    size_t suf_len = strlen(suffix);
    char *out = malloc(base_len + suf_len + 1);
    if (!out) return NULL;
    memcpy(out, input_path, base_len);
    memcpy(out + base_len, suffix, suf_len + 1);
    return out;
}

static char *xml_node_text(xmlNodePtr node) {
    xmlChar *content = xmlNodeGetContent(node);
    if (!content) return strdup("");
    char *s = strdup((const char *)content);
    xmlFree(content);
    return s;
}

static int parse_replace(xmlNodePtr replace, Fixup *out) {
    int got_type = 0, got_name = 0, got_value = 0, got_repl = 0;
    for (xmlNodePtr child = replace->children; child; child = child->next) {
        if (child->type != XML_ELEMENT_NODE) continue;
        char *text = xml_node_text(child);
        if (strcmp((const char *)child->name, "type") == 0) {
            if (strcmp(text, "element") == 0) {
                out->type = 0;
                got_type = 1;
            } else if (strcmp(text, "attribute") == 0) {
                out->type = 1;
                got_type = 1;
            }
        } else if (strcmp((const char *)child->name, "name") == 0) {
            out->name = strdup(text);
            got_name = 1;
        } else if (strcmp((const char *)child->name, "value") == 0) {
            out->value = strdup(text);
            got_value = 1;
        } else if (strcmp((const char *)child->name, "replacement-value") == 0) {
            out->replacement_value = strdup(text);
            got_repl = 1;
        }
        free(text);
    }
    return got_type && got_name && got_value && got_repl;
}

FixupList fixer_load(const char *input_file_path) {
    FixupList fl = {NULL, 0};
    char *path = fixup_sidecar_path(input_file_path);
    if (!path) return fl;

    FILE *f = fopen(path, "r");
    if (!f) {
        free(path);
        return fl;
    }
    fclose(f);

    xmlDocPtr doc = xmlReadFile(path, NULL, XML_PARSE_NONET);
    free(path);
    if (!doc) return fl;

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root || strcmp((const char *)root->name, "fixups") != 0) {
        xmlFreeDoc(doc);
        return fl;
    }

    int cap = 0;
    for (xmlNodePtr child = root->children; child; child = child->next) {
        if (child->type != XML_ELEMENT_NODE) continue;
        if (strcmp((const char *)child->name, "replace") != 0) continue;
        Fixup fx = {0, NULL, NULL, NULL};
        if (parse_replace(child, &fx)) {
            if (fl.count >= cap) {
                cap = cap ? cap * 2 : 8;
                fl.items = realloc(fl.items, sizeof(Fixup) * cap);
            }
            fl.items[fl.count++] = fx;
        } else {
            free(fx.name);
            free(fx.value);
            free(fx.replacement_value);
        }
    }
    xmlFreeDoc(doc);
    return fl;
}

void fixer_apply_to_expected(const FixupList *fl, xmlNodePtr node) {
    if (!fl->count || !node) return;
    if (node->type != XML_ELEMENT_NODE) return;

    int has_children = 0;
    for (xmlNodePtr c = node->children; c; c = c->next) {
        if (c->type == XML_ELEMENT_NODE) {
            has_children = 1;
            break;
        }
    }

    if (!has_children) {
        char *text = xml_node_text(node);
        for (int i = 0; i < fl->count; i++) {
            if (fl->items[i].type != 0) continue;
            if (strcmp((const char *)node->name, fl->items[i].name) != 0) continue;
            if (strcmp(text, fl->items[i].value) == 0) {
                xmlNodeSetContent(node, (const xmlChar *)fl->items[i].replacement_value);
                break;
            }
        }
        free(text);
    }

    for (xmlAttrPtr attr = node->properties; attr; attr = attr->next) {
        xmlChar *val = xmlGetProp(node, attr->name);
        if (!val) continue;
        for (int i = 0; i < fl->count; i++) {
            if (fl->items[i].type != 1) continue;
            if (strcmp((const char *)attr->name, fl->items[i].name) != 0) continue;
            if (strcmp((const char *)val, fl->items[i].value) == 0) {
                xmlSetProp(node, attr->name,
                           (const xmlChar *)fl->items[i].replacement_value);
                break;
            }
        }
        xmlFree(val);
    }

    for (xmlNodePtr child = node->children; child; child = child->next) {
        if (child->type == XML_ELEMENT_NODE)
            fixer_apply_to_expected(fl, child);
    }
}

void fixer_free(FixupList *fl) {
    for (int i = 0; i < fl->count; i++) {
        free(fl->items[i].name);
        free(fl->items[i].value);
        free(fl->items[i].replacement_value);
    }
    free(fl->items);
    fl->items = NULL;
    fl->count = 0;
}
