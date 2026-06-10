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

/* The MusicXML version the generated model supports comes from the model
   itself (MX_SUPPORTED_MUSICXML_VERSION in mx_runtime.h, emitted from the
   schema the config pins), so retargeting the schema cannot leave this gate
   stale. Documents declaring a NEWER version may use elements the model has
   no types for; MusicXML is backward compatible, so older documents are
   fine. An absent version attribute means MusicXML 1.0. */
static int declared_version_exceeds(xmlDocPtr doc) {
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root)
        return 0;
    int max_major = 0, max_minor = 0;
    sscanf(MX_SUPPORTED_MUSICXML_VERSION, "%d.%d", &max_major, &max_minor);
    xmlChar *version = xmlGetProp(root, (const xmlChar *)"version");
    int major = 1, minor = 0;
    if (version && version[0])
        sscanf((const char *)version, "%d.%d", &major, &minor);
    xmlFree(version);
    return major > max_major || (major == max_major && minor > max_minor);
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
                 "declares MusicXML > %s; this target generates from the "
                 "%s schema",
                 MX_SUPPORTED_MUSICXML_VERSION, MX_SUPPORTED_MUSICXML_VERSION);
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
