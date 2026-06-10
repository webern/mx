#include "roundtrip.h"

#include "compare.h"
#include "fixer.h"
#include "normalize.h"

#include "mx_document.h"
#include "mx_runtime.h"

#include <libxml/parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The MusicXML version the generated model supports (the schema pinned in
   this target's config.toml). Documents declaring a NEWER version may use
   elements the model has no types for; MusicXML is backward compatible, so
   older documents are fine. */
#define MAX_SUPPORTED_MAJOR 3
#define MAX_SUPPORTED_MINOR 1

/* Reports whether the root's version attribute declares a version newer
   than the supported one. An absent attribute means MusicXML 1.0. */
static int declared_version_exceeds(xmlDocPtr doc) {
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root)
        return 0;
    xmlChar *version = xmlGetProp(root, (const xmlChar *)"version");
    int major = 1, minor = 0;
    if (version && version[0])
        sscanf((const char *)version, "%d.%d", &major, &minor);
    xmlFree(version);
    return major > MAX_SUPPORTED_MAJOR ||
           (major == MAX_SUPPORTED_MAJOR && minor > MAX_SUPPORTED_MINOR);
}

RoundtripResult run_core_roundtrip(const char *abs_input_path) {
    RoundtripResult r = {0, 0, ""};

    xmlDocPtr input_doc = xmlReadFile(abs_input_path, NULL, XML_PARSE_NONET);
    if (!input_doc) {
        snprintf(r.message, sizeof(r.message), "failed to load: %s",
                 abs_input_path);
        return r;
    }

    if (declared_version_exceeds(input_doc)) {
        snprintf(r.message, sizeof(r.message),
                 "declares MusicXML > %d.%d; this target generates from the "
                 "%d.%d schema",
                 MAX_SUPPORTED_MAJOR, MAX_SUPPORTED_MINOR,
                 MAX_SUPPORTED_MAJOR, MAX_SUPPORTED_MINOR);
        r.skipped = 1;
        xmlFreeDoc(input_doc);
        return r;
    }

    set_root_version(xmlDocGetRootElement(input_doc));

    MxDocument *model = mx_document_from_xdoc(input_doc);
    if (!model) {
        snprintf(r.message, sizeof(r.message), "from_xdoc: %s", mx_error());
        xmlFreeDoc(input_doc);
        return r;
    }

    xmlDocPtr actual_doc = NULL;
    if (mx_document_to_xdoc(model, &actual_doc) != 0) {
        snprintf(r.message, sizeof(r.message), "to_xdoc: %s", mx_error());
        mx_document_free(model);
        xmlFreeDoc(input_doc);
        return r;
    }
    mx_document_free(model);
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
