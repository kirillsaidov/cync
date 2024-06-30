#include "schema.h"

/**
 * @brief  Find string in list
 * @note   
 * @param  s: string
 * @param  list: list of strings
 * @param  method: 0: equals s==list(str), 1: can find s in list(str), 2: list(str) ends with s
 * @retval true if str in list else false
 */
static bool cync_find_str_in_list(const vt_str_t *const s, vt_plist_t *const list, const int method);

void cync_schema_master_slave(const char *const master, const char *const slave, const bool ignore_df, const bool verbose) {
    if (verbose) cync_log("Starting...");

    // create alloctor
    vt_mallocator_t *alloctr = vt_mallocator_create();

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
    dir_master = vt_path_dir_list_recurse(dir_master, master, ignore_df);
    dir_slave = vt_path_dir_list_recurse(dir_slave, slave, ignore_df);
    
    // sync files
    void *path = NULL;
    while ((path = vt_plist_slide_front(dir_master)) != NULL) {
        vt_str_t *item = path;
        
        // check if item exists in slave
        vt_str_t *basename = vt_str_create_len(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
        if (cync_find_str_in_list(vt_path_basename(basename, item), dir_slave, 2)) {
            // check modification date
            // copy if needed
        } else {
            // check if file
            if (vt_path_is_file(vt_str_z(item))) vt_path_pop(vt_str_z(item));
            
            // mkdirs
            vt_path_mkdir_parents(vt_str_z(item));

            // copy file
        }
    } 

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

    // free resources
    vt_mallocator_destroy(alloctr);
}


static bool cync_find_str_in_list(const vt_str_t *const s, vt_plist_t *const list, const int method) {
    VT_DEBUG_ASSERT(s != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // iterate over list and find str
    void *path = NULL;
    while ((path = vt_plist_slide_front(list)) != NULL) {
        vt_str_t *item = path;
        if (method == 0 && vt_str_equals(s, item)) {
            vt_plist_slide_reset(list);
            return true;
        } else if (method == 1 && vt_str_can_find(item, vt_str_z(s))) {
            vt_plist_slide_reset(list);
            return true;
        } else if (vt_str_ends_with(item, vt_str_z(s))) {
            vt_plist_slide_reset(list);
            return true;
        }
    } 

    return false;
}


