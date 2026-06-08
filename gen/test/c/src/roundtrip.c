#include "roundtrip.h"

#include "compare.h"
#include "fixer.h"
#include "normalize.h"
#include "stub.h"

#include <libxml/parser.h>
#include <stdio.h>
#include <string.h>

RoundtripResult run_core_roundtrip(const char *abs_input_path) {
    RoundtripResult r = {0, ""};

    xmlDocPtr input_doc = xmlReadFile(abs_input_path, NULL, XML_PARSE_NONET);
    if (!input_doc) {
        snprintf(r.message, sizeof(r.message), "failed to load: %s",
                 abs_input_path);
        return r;
    }

    void *model = NULL;
    if (mx_from_xdoc(input_doc, &model) != 0) {
        snprintf(r.message, sizeof(r.message),
                 "from_xdoc: generated parser not implemented");
        xmlFreeDoc(input_doc);
        return r;
    }

    xmlDocPtr actual_doc = NULL;
    if (mx_to_xdoc(model, &actual_doc) != 0) {
        snprintf(r.message, sizeof(r.message), "to_xdoc failed");
        mx_model_free(model);
        xmlFreeDoc(input_doc);
        return r;
    }
    mx_model_free(model);
    normalize(actual_doc);

    xmlDocPtr expected_doc = xmlReadFile(abs_input_path, NULL, XML_PARSE_NONET);
    if (!expected_doc) {
        snprintf(r.message, sizeof(r.message), "failed to reload expected");
        xmlFreeDoc(actual_doc);
        xmlFreeDoc(input_doc);
        return r;
    }
    normalize(expected_doc);

    FixupList fl = fixer_load(abs_input_path);
    if (fl.count > 0)
        fixer_apply_to_expected(&fl, xmlDocGetRootElement(expected_doc));
    fixer_free(&fl);

    CompareResult cmp = compare_elements(
        xmlDocGetRootElement(expected_doc),
        xmlDocGetRootElement(actual_doc));

    if (cmp.failed) {
        snprintf(r.message, sizeof(r.message), "%s", cmp.detail);
    } else {
        r.ok = 1;
    }

    xmlFreeDoc(expected_doc);
    xmlFreeDoc(actual_doc);
    xmlFreeDoc(input_doc);
    return r;
}
