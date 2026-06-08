#include "discover.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char *excluded_segments[] = {
    "expected", "testOutput", "generalxml", "smufl", NULL};

static int is_excluded_path(const char *rel) {
    for (const char **seg = excluded_segments; *seg; seg++) {
        const char *p = rel;
        while (*p) {
            const char *slash = strchr(p, '/');
            int len = slash ? (int)(slash - p) : (int)strlen(p);
            if (len == (int)strlen(*seg) && strncmp(p, *seg, len) == 0)
                return 1;
            if (!slash) break;
            p = slash + 1;
        }
    }
    return 0;
}

static int has_xml_extension(const char *path) {
    const char *dot = strrchr(path, '.');
    if (!dot) return 0;
    return strcmp(dot, ".xml") == 0 || strcmp(dot, ".musicxml") == 0;
}

static int is_fixup_sidecar(const char *path) {
    const char *suffix = ".fixup.xml";
    size_t slen = strlen(suffix);
    size_t plen = strlen(path);
    if (plen < slen) return 0;
    return strcmp(path + plen - slen, suffix) == 0;
}

static int has_invalid_marker(const char *path) {
    size_t len = strlen(path);
    char *marker = malloc(len + 9);
    if (!marker) return 0;
    sprintf(marker, "%s.invalid", path);
    struct stat st;
    int exists = stat(marker, &st) == 0;
    free(marker);
    return exists;
}

static int cmp_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

static void walk_dir(const char *dir, const char *data_root,
                     char ***out, int *count, int *cap) {
    DIR *d = opendir(dir);
    if (!d) return;

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;

        size_t path_len = strlen(dir) + 1 + strlen(ent->d_name) + 1;
        char *path = malloc(path_len);
        snprintf(path, path_len, "%s/%s", dir, ent->d_name);

        struct stat st;
        if (stat(path, &st) != 0) {
            free(path);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            walk_dir(path, data_root, out, count, cap);
            free(path);
            continue;
        }

        if (!S_ISREG(st.st_mode)) {
            free(path);
            continue;
        }

        size_t root_len = strlen(data_root);
        const char *rel = path + root_len;
        if (*rel == '/') rel++;

        if (is_excluded_path(rel) || !has_xml_extension(path) ||
            is_fixup_sidecar(path) || has_invalid_marker(path)) {
            free(path);
            continue;
        }

        if (*count >= *cap) {
            *cap = *cap ? *cap * 2 : 256;
            *out = realloc(*out, sizeof(char *) * *cap);
        }
        (*out)[(*count)++] = path;
    }
    closedir(d);
}

FileList discover_input_files(const char *data_root) {
    FileList fl = {NULL, 0};
    int cap = 0;
    walk_dir(data_root, data_root, &fl.paths, &fl.count, &cap);
    if (fl.count > 1)
        qsort(fl.paths, fl.count, sizeof(char *), cmp_strings);
    return fl;
}

void file_list_free(FileList *list) {
    for (int i = 0; i < list->count; i++)
        free(list->paths[i]);
    free(list->paths);
    list->paths = NULL;
    list->count = 0;
}

char *to_test_name(const char *abs_path, const char *data_root) {
    size_t root_len = strlen(data_root);
    if (strncmp(abs_path, data_root, root_len) != 0)
        return strdup(abs_path);
    const char *rel = abs_path + root_len;
    if (*rel == '/') rel++;
    return strdup(rel);
}
