#include "schema.h"

void cync_schema_master_slave(const char *const master, const char *const slave, const bool verbose) {
    if (verbose) cync_log("Starting...");

    // expand tilda if neccessary
    vt_str_t *path_master = vt_path_expand_tilda(master, alloctr);
    vt_str_t *path_slave = vt_path_expand_tilda(slave, alloctr);

    // check if path exists
    if (!vt_path_exists(vt_str_z(path_master))) {
        cync_log("Source path does not exist!");
        return;
    }
    if (!vt_path_exists(vt_str_z(path_slave))) {
        cync_log("Destination path does not exist!");
        return;
    }
    
    // list directories
    vt_plist_t *dir_master = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    vt_plist_t *dir_slave = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    dir_master = vt_path_dir_list_recurse(dir_master, master, false);
    dir_slave = vt_path_dir_list_recurse(dir_slave, slave, false);

    cync_log("master:");
    void *path = NULL;
    while ((path = vt_plist_slide_front(dir_master)) != NULL) {
        vt_str_t *item = path;
        printf("%s\n", vt_str_z(item));
    }

    cync_log("slave:");
    path = NULL;
    while ((path = vt_plist_slide_front(dir_slave)) != NULL) {
        vt_str_t *item = path;
        printf("%s\n", vt_str_z(item));
    }

}

