#include "mode.h"

void cync_mode_target(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create directory tree for dst
    cync_tools_create_dirtree(src, dst, ignore_df, verbose, alloctr);

    // copy or update files from src to dst
    cync_tools_copy_update_files(src, dst, ignore_df, low_mem, verbose, alloctr);

    // remove files from destination if they are absent in source
    cync_tools_remove_files(src, dst, ignore_df, verbose, alloctr);

    // remove directory tree from dst if absent from src
    cync_tools_remove_dirtree(src, dst, ignore_df, verbose, alloctr);
}

/*
void cync_mode_dual(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    return;
}

void cync_mode_auto(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    return;
}

void cync_mode_net(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    return;
}
*/


/*void __cync_mode_target(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose) {
    if (verbose) cync_log_ln("Starting syncronization process...");

    // create alloctor
    vt_mallocator_t *alloctr = vt_mallocator_create();

    // expand tilda if neccessary
    vt_str_t *path_src = vt_path_expand_tilda(src, alloctr);
    vt_str_t *path_dst = vt_path_expand_tilda(dst, alloctr);

    // check if path exists
    if (!vt_path_exists(vt_str_z(path_src))) {
        if (verbose) cync_log("Source path does not exist!");
        return;
    }
    if (!vt_path_exists(vt_str_z(path_dst))) {
        if (verbose) cync_log("Destination path does not exist!");
        return;
    }
    
    // list directories
    vt_plist_t *dir_src = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    vt_plist_t *dir_dst = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    dir_src = vt_path_dir_list_recurse(dir_src, vt_str_z(path_src), ignore_df);
    dir_dst = vt_path_dir_list_recurse(dir_dst, vt_str_z(path_dst), ignore_df);

    // create dst dirtree if does not exist
    void *path = NULL;
    vt_plist_t *dirs = cync_filter_dirs(dir_src);
    while ((path = vt_plist_slide_front(dirs)) != NULL) {
        vt_str_t *item = path;
        
        // replace src dirtree with dst dirtree
        vt_str_replace_first(item, vt_str_z(path_src), vt_str_z(path_dst));

        // create dirtree in dst
        if (!vt_path_exists(vt_str_z(item))) {
            const bool ret = vt_path_mkdir_parents(vt_str_z(item));
            if (verbose) cync_log_ln("MKDIR(%s) <%s>", ret ? "OK" : "ER", vt_str_z(item));
        }
    }

    // sync files
    path = NULL;
    vt_str_t *basename = vt_str_create_len(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    while ((path = vt_plist_slide_front(dir_src)) != NULL) {
        // cast to vt_str_t
        vt_str_t *item = path;

        // skip directories
        if (vt_path_is_dir(vt_str_z(item))) continue;
        
        // get file basename
        basename = vt_path_basename(basename, vt_str_z(item));
        
        // create dst file and modify to fit dst directory tree
        vt_str_t *dst_file = vt_str_dup(item);
        vt_str_replace_first(dst_file, vt_str_z(path_src), vt_str_z(path_dst));

        // check if item exists in dst
        if (cync_find_str_in_list(basename, dir_dst, 2)) {
            // get modification date
            const time_t modiftime_src = cync_file_time_modified(vt_str_z(item));
            const time_t modiftime_dst = cync_file_time_modified(vt_str_z(dst_file));
            
            // copy if needed
            if (modiftime_src > modiftime_dst) {
                // copy file
                const bool ret = cync_copy_file(vt_str_z(item), vt_str_z(dst_file), low_mem);
                if (verbose) cync_log_ln("UPDATE(%s) <%s>", ret ? "OK" : "ER", vt_str_z(dst_file));
            }
        } else {
            // copy file
            if (verbose) cync_log_ln("COPY <%s>", vt_str_z(item));
            const bool ret = cync_copy_file(vt_str_z(item), vt_str_z(dst_file), low_mem);
            if (verbose) cync_log_ln("(%s) <%s>", ret ? "OK" : "ER", vt_str_z(dst_file));
        }
    }

    // delete unneccessary files
    path = NULL;
    while ((path = vt_plist_slide_front(dir_dst)) != NULL) {
        // cast to vt_str_t
        vt_str_t *item = path;

        // get file basename
        basename = vt_path_basename(basename, vt_str_z(item));
        
        // check if item exists in src
        if (!cync_find_str_in_list(basename, dir_src, 2)) {            
            // delete file or directory
            const bool ret = vt_path_is_dir(vt_str_z(item)) ? 
                vt_path_rmdir(vt_str_z(item)) : 
                vt_path_remove(vt_str_z(item));
            if (verbose) cync_log_ln("REMOVE(%s) <%s>", ret ? "OK" : "ER", vt_str_z(item));
        }
    }

    // free resources
    vt_mallocator_destroy(alloctr);
    if (verbose) cync_log_ln("All files up-to-date.");
}
*/

