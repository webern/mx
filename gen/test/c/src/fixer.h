#ifndef MX_CORERT_FIXER_H
#define MX_CORERT_FIXER_H

#include <libxml/tree.h>

typedef struct {
    int type; /* 0 = element, 1 = attribute */
    char *name;
    char *value;
    char *replacement_value;
} Fixup;

typedef struct {
    Fixup *items;
    int count;
} FixupList;

FixupList fixer_load(const char *input_file_path);
void fixer_apply_to_expected(const FixupList *fl, xmlNodePtr node);
void fixer_free(FixupList *fl);

#endif
