#include "schema.h"

void cync_schema_master_slave(const char *const master, const char *const slave, const bool ignore_df, const bool verbose) {
    if (verbose) cync_log_ln("Starting syncronization process...");

    // create alloctor
    vt_mallocator_t *alloctr = vt_mallocator_create();

    // expand tilda if neccessary
    vt_str_t *path_master = vt_path_expand_tilda(master, alloctr);
    vt_str_t *path_slave = vt_path_expand_tilda(slave, alloctr);

    // check if path exists
    if (!vt_path_exists(vt_str_z(path_master))) {
        if (verbose) cync_log("Source path does not exist!");
        return;
    }
    if (!vt_path_exists(vt_str_z(path_slave))) {
        if (verbose) cync_log("Destination path does not exist!");
        return;
    }
    
    // list directories
    vt_plist_t *dir_master = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    vt_plist_t *dir_slave = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    dir_master = vt_path_dir_list_recurse(dir_master, vt_str_z(path_master), ignore_df);
    dir_slave = vt_path_dir_list_recurse(dir_slave, vt_str_z(path_slave), ignore_df);

    // create slave dirtree if does not exist
    void *path = NULL;
    vt_plist_t *dirs = cync_filter_dirs(dir_master);
    while ((path = vt_plist_slide_front(dirs)) != NULL) {
        vt_str_t *item = path;
        
        // replace master dirtree with slave dirtree
        vt_str_replace_first(item, vt_str_z(path_master), vt_str_z(path_slave));

        // create dirtree in slave
        if (!vt_path_exists(vt_str_z(item))) {
            const bool ret = vt_path_mkdir_parents(vt_str_z(item));
            if (verbose) cync_log_ln("MKDIR(%s) <%s>", ret ? "OK" : "ER", vt_str_z(item));
        }
    }

    // sync files
    path = NULL;
    while ((path = vt_plist_slide_front(dir_master)) != NULL) {
        // get cast to vt_str_t
        vt_str_t *item = path;

        // skip directories
        if (vt_path_is_dir(vt_str_z(item))) continue;
        
        // get file basename
        vt_str_t *basename = vt_str_create_len(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
        basename = vt_path_basename(basename, vt_str_z(item));
        
        // create slave file and modify to fit slave directory tree
        vt_str_t *slave_file = vt_str_dup(item);
        vt_str_replace_first(slave_file, vt_str_z(path_master), vt_str_z(path_slave));

        // check if item exists in slave
        if (cync_find_str_in_list(basename, dir_slave, 2)) {
            // get modification date
            const time_t modiftime_master = cync_file_time_modified(vt_str_z(item));
            const time_t modiftime_slave = cync_file_time_modified(vt_str_z(slave_file));
            
            // copy if needed
            if (modiftime_master > modiftime_slave) {
                // copy file
                const bool ret = cync_copy_file(vt_str_z(item), vt_str_z(slave_file));
                if (verbose) cync_log_ln("UPDATE(%s) <%s>", ret ? "OK" : "ER", vt_str_z(slave_file));
            }
        } else {
            // skip if file exists
            if (vt_path_exists(vt_str_z(slave_file))) continue;

            // copy file
            if (verbose) cync_log_ln("COPY <%s>", vt_str_z(item));
            const bool ret = cync_copy_file(vt_str_z(item), vt_str_z(slave_file));
            if (verbose) cync_log_ln("(%s) <%s>", ret ? "OK" : "ER", vt_str_z(slave_file));
        }
    }

    // free resources
    vt_mallocator_destroy(alloctr);
    if (verbose) cync_log_ln("All files up-to-date.");
}


