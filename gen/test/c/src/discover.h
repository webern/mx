#ifndef MX_CORERT_DISCOVER_H
#define MX_CORERT_DISCOVER_H

typedef struct {
    char **paths;
    int count;
} FileList;

FileList discover_input_files(const char *data_root);
void file_list_free(FileList *list);

char *to_test_name(const char *abs_path, const char *data_root);

#endif
