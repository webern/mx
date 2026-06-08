#ifndef MX_CORERT_STUB_H
#define MX_CORERT_STUB_H

#include <libxml/tree.h>

// Stub for the generated parser. Returns non-zero (failure) until the
// generator emits the C typed model.
int mx_from_xdoc(xmlDocPtr input, void **model_out);
int mx_to_xdoc(void *model, xmlDocPtr *output);
void mx_model_free(void *model);

#endif
