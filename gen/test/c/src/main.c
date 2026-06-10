#include "discover.h"
#include "roundtrip.h"

#include <libxml/parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MX_REPO_ROOT
#error "MX_REPO_ROOT must be defined at compile time"
#endif

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    LIBXML_TEST_VERSION

    char data_root[4096];
    snprintf(data_root, sizeof(data_root), "%s/data", MX_REPO_ROOT);

    FileList files = discover_input_files(data_root);
    if (files.count == 0) {
        fprintf(stderr, "no input files discovered under %s\n", data_root);
        return 1;
    }

    int pass = 0, fail = 0, skip = 0;
    for (int i = 0; i < files.count; i++) {
        char *name = to_test_name(files.paths[i], data_root);
        RoundtripResult result = run_core_roundtrip(files.paths[i]);
        if (result.skipped) {
            skip++;
        } else if (result.ok) {
            pass++;
        } else {
            fail++;
            printf("FAIL %s: %s\n", name, result.message);
        }
        free(name);
    }

    printf("\n%d tests: %d passed, %d failed, %d skipped\n",
           pass + fail + skip, pass, fail, skip);
    file_list_free(&files);
    xmlCleanupParser();
    return fail > 0 ? 1 : 0;
}
