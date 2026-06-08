#ifndef MX_CORERT_COMPARE_H
#define MX_CORERT_COMPARE_H

#include <libxml/tree.h>

typedef struct {
    int failed;
    char detail[1024];
} CompareResult;

CompareResult compare_elements(xmlNodePtr expected, xmlNodePtr actual);

#endif
