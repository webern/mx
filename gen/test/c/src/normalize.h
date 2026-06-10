#ifndef MX_CORERT_NORMALIZE_H
#define MX_CORERT_NORMALIZE_H

#include <libxml/tree.h>

void normalize(xmlDocPtr doc);

/* Pin the root version attribute to the corert baseline ("3.0"). */
void set_root_version(xmlNodePtr root);

#endif
