#include "stub.h"

int mx_from_xdoc(xmlDocPtr input, void **model_out) {
    (void)input;
    *model_out = NULL;
    return -1; /* not implemented */
}

int mx_to_xdoc(void *model, xmlDocPtr *output) {
    (void)model;
    *output = NULL;
    return -1; /* not implemented */
}

void mx_model_free(void *model) {
    (void)model;
}
