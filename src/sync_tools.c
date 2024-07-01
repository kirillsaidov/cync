#include "sync_tools.h"

bool cync_find_str_in_list(const vt_str_t *const s, vt_plist_t *const list, const int method) {
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

bool cync_copy_file(const char *const src, const char *const dst) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // open read/write file
    FILE *src_file = fopen(src, "rb");
    FILE *dst_file = fopen(dst, "wb");

    // check if file is empty
    bool success = true;
    const size_t filesize = vt_path_get_file_size(src);
    if (filesize) {
        // copy the contents from source to destination
        size_t bytesRead = 0;
        const size_t buffer_size = filesize < CYNC_ONE_MB * 100 ? CYNC_ONE_MB * 10 : CYNC_ONE_MB * 100;
        char *buffer = VT_CALLOC(buffer_size);
        while ((bytesRead = fread(buffer, 1, buffer_size, src_file)) > 0) {
            if (fwrite(buffer, 1, bytesRead, dst_file) != bytesRead) {
                cync_log("Error writing to destination file!");
                success = false;
                break;
            }
        }
        VT_FREE(buffer);

        // check for read error
        if (ferror(src_file)) {
            cync_log("Error reading from source file!");
            success = false;
        }
    }

    // close the files
    fclose(src_file);
    fclose(dst_file);

    return success;
}

vt_plist_t *cync_filter_dirs(vt_plist_t *const list) {
    VT_DEBUG_ASSERT(list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create directory list
    vt_plist_t *dirlist = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, list->alloctr);

    void *path = NULL;
    while ((path = vt_plist_slide_front(list)) != NULL) {
        vt_str_t *item = path;
        if (vt_path_is_dir(vt_str_z(item))) vt_plist_push_back(dirlist, vt_str_dup(item));
    } 

    return dirlist;
}

time_t cync_file_time_modified(const char *const filepath) {
    struct stat file_stat;

    // get file statistics
    #if defined(_WIN32) || defined(_WIN64)
        if (_stat(filepath, &file_stat) != 0) return 0;
    #else
        if (stat(filepath, &file_stat) != 0) return 0;
    #endif

    // Return the modification time
    return file_stat.st_mtime;
}


