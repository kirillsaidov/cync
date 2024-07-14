#include "tools.h"
#include <utime.h>

static bool cync_tools_filter_is_dir(void *spath);
static bool cync_tools_filter_is_file(void *spath);

vt_plist_t *cync_tools_filter_list(vt_plist_t *const list, bool (*filter)(void*), vt_mallocator_t *const alloctr) {
    VT_DEBUG_ASSERT(list != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(filter != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // create filtered list
    vt_plist_t *filtered_list = NULL;

    // iterate over list and find str
    void *item = NULL;
    while ((item = vt_plist_slide_front(list)) != NULL) {
        if (filter(item)) {
            // allocate if necessary
            filtered_list = filtered_list ? filtered_list : vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);

            // append to new list
            vt_plist_push_back(filtered_list, item);
        }
    }

    return filtered_list;
}

bool cync_tools_copy_file(const char *const src, const char *const dst, const bool low_mem) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(!vt_str_equals_z(src, dst), "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(vt_path_exists(src), "File does not exist: %s!\n", src);

    // open read/write file
    FILE *src_file = fopen(src, "rb");
    FILE *dst_file = fopen(dst, "wb");

    // check if file is empty
    bool success = true;
    const size_t filesize = vt_path_get_file_size(src);
    if (filesize) {
        // copy the contents from source to destination
        size_t bytesRead = 0;
        const size_t buffer_size = low_mem ? 
            CYNC_COPY_SIZE_LOW_MEM : 
            filesize < CYNC_COPY_SIZE_LARGE ?
            CYNC_COPY_SIZE_DEFAULT : 
            CYNC_COPY_SIZE_LARGE;
        char *buffer = VT_CALLOC(buffer_size);
        while ((bytesRead = fread(buffer, 1, buffer_size, src_file)) > 0) {
            if (fwrite(buffer, 1, bytesRead, dst_file) != bytesRead) {
                cync_log_ln("Error writing to destination file!");
                success = false;
                break;
            }
        }
        VT_FREE(buffer);

        // check for read error
        if (ferror(src_file)) {
            cync_log_ln("Error reading from source file!");
            success = false;
        }
    }

    // close the files
    fclose(src_file);
    fclose(dst_file);

    // get source file attributes
    struct stat src_stat;
    #if defined(_WIN32) || defined(_WIN64)
        if (!success || _stat(src, &src_stat) != 0) success = false;
    #else
        if (!success || stat(src, &src_stat) != 0) success = false;
    #endif 

    // set file ownership of the destination file
    if (!success || chown(dst, src_stat.st_uid, src_stat.st_gid) != 0) {
        cync_log_ln("Failed setting file permissions!");
        success = false;
    }

    // set access and modification times
    struct utimbuf dst_utime_buf = { .actime = src_stat.st_atime, .modtime = src_stat.st_mtime };
    if (utime(dst, &dst_utime_buf) != 0) {
        cync_log_ln("Failed to set access and modification times");
        exit(EXIT_FAILURE);
    }

    return success;
}

time_t cync_tools_get_filetime_modified(const char *const filepath) {
    VT_DEBUG_ASSERT(filepath != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_ENFORCE(vt_path_exists(filepath), "File does not exist: %s!\n", filepath);

    // get file statistics
    struct stat file_stat;
    #if defined(_WIN32) || defined(_WIN64)
        if (_stat(filepath, &file_stat) != 0) return 0;
    #else
        if (stat(filepath, &file_stat) != 0) return 0;
    #endif

    // return the modification time
    return file_stat.st_mtime;
}

void cync_tools_create_dirtree(const char *const src, const char *const dst, const bool ignore_df, const bool verbose, vt_mallocator_t *const alloctr) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // setup
    vt_plist_t *src_contents = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);

    // get directory contents
    src_contents = vt_path_dir_list_recurse(src_contents, src, ignore_df);

    // filter for directories
    vt_plist_t *src_dirs = cync_tools_filter_list(src_contents, cync_tools_filter_is_dir, alloctr);
    if (!src_dirs) return;

    // iterate over src directory and create dirtree in destination
    void *item = NULL;
    while ((item = vt_plist_slide_front(src_dirs)) != NULL) {
        // cast to vt_str_t
        vt_str_t *spath = item;

        // replace with src path with dst path
        vt_str_replace_first(spath, src, dst);

        // check if dst directory exists, create otherwise
        if (!vt_path_exists(vt_str_z(spath))) {
            const bool ret = vt_path_mkdir_parents(vt_str_z(spath));
            if (verbose) cync_log_ln("MKDIR(%s) <%s>", ret ? "OK" : "ER", vt_str_z(spath));
        }
    }
}

void cync_tools_copy_update_files(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose, vt_mallocator_t *const alloctr) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // setup
    vt_plist_t *src_contents = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);

    // get directory contents
    src_contents = vt_path_dir_list_recurse(src_contents, src, ignore_df);

    // filter for files
    vt_plist_t *src_files = cync_tools_filter_list(src_contents, cync_tools_filter_is_file, alloctr);
    if (!src_files) return;

    // iterate over src directory and create dirtree in destination
    void *item = NULL;
    vt_str_t *dst_file = vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    while ((item = vt_plist_slide_front(src_files)) != NULL) {
        // cast to vt_str_t
        vt_str_t *src_file = item;

        // create dst path from src path
        vt_str_clear(dst_file);
        vt_str_append(dst_file, vt_str_z(src_file));
        vt_str_replace_first(dst_file, src, dst);

        // check if dst file exists, copy otherwise
        if (!vt_path_exists(vt_str_z(dst_file))) {
            const bool ret = cync_tools_copy_file(vt_str_z(src_file), vt_str_z(dst_file), low_mem);
            if (verbose) {
                cync_log_ln("COPY <%s>", vt_str_z(src_file));
                cync_log_ln("(%s) <%s>", ret ? "OK" : "ER", vt_str_z(dst_file));
            }
        } else { // update file
            // get modification dates
            const time_t src_mtime = cync_tools_get_filetime_modified(vt_str_z(src_file));
            const time_t dst_mtime = cync_tools_get_filetime_modified(vt_str_z(dst_file));
            const time_t timedelta = (time_t)difftime(src_mtime, dst_mtime);

            // check if we need to update the file
            if (timedelta > 10) { // 1 seconds = 10 (in value)
                const bool ret = cync_tools_copy_file(vt_str_z(src_file), vt_str_z(dst_file), low_mem);
                if (verbose) cync_log_ln("UPDATE(%s) <%s>", ret ? "OK" : "ER", vt_str_z(dst_file));
            }
        }
    }
}

void cync_tools_remove_files(const char *const src, const char *const dst, const bool ignore_df, const bool verbose, vt_mallocator_t *const alloctr) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // setup
    vt_plist_t *dst_contents = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);

    // get directory contents
    dst_contents = vt_path_dir_list_recurse(dst_contents, dst, ignore_df);

    // filter for files
    vt_plist_t *dst_files = cync_tools_filter_list(dst_contents, cync_tools_filter_is_file, alloctr);
    if (!dst_files) return;

    // iterate over src directory and create dirtree in destination
    void *item = NULL;
    vt_str_t *src_file = vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    while ((item = vt_plist_slide_front(dst_files)) != NULL) {
        // cast to vt_str_t
        vt_str_t *dst_file = item;

        // create dst path from src path
        vt_str_clear(src_file);
        vt_str_append(src_file, vt_str_z(dst_file));
        vt_str_replace_first(src_file, dst, src);

        // check if file exists in source directory, otherwise remove it from destination directory
        if (!vt_path_exists(vt_str_z(src_file))) {
            const bool ret = vt_path_remove(vt_str_z(dst_file));
            if (verbose) cync_log_ln("REMOVE(%s) <%s>", ret ? "OK" : "ER", vt_str_z(dst_file));
        }
    }
}

void cync_tools_remove_dirtree(const char *const src, const char *const dst, const bool ignore_df, const bool verbose, vt_mallocator_t *const alloctr) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));

    // setup
    vt_plist_t *dst_contents = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);

    // get directory contents
    dst_contents = vt_path_dir_list_recurse(dst_contents, dst, ignore_df);

    // filter for files
    vt_plist_t *dst_dirs = cync_tools_filter_list(dst_contents, cync_tools_filter_is_dir, alloctr); 
    if (!dst_dirs) return;

    // iterate over src directory and create dirtree in destination
    void *item = NULL;
    vt_str_t *src_dir = vt_str_create_capacity(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    while ((item = vt_plist_slide_front(dst_dirs)) != NULL) {
        // cast to vt_str_t
        vt_str_t *dst_dir = item;

        // create dst path from src path
        vt_str_clear(src_dir);
        vt_str_append(src_dir, vt_str_z(dst_dir));
        vt_str_replace_first(src_dir, dst, src);

        // check if file exists in source directory, otherwise remove it from destination directory
        if (!vt_path_exists(vt_str_z(src_dir))) {
            const bool ret = vt_path_rmdir(vt_str_z(dst_dir));
            if (verbose) cync_log_ln("RMDIR(%s) <%s>", ret ? "OK" : "ER", vt_str_z(dst_dir));
        }
    }
}

/* -------------------- PRIVATE -------------------- */

/// @brief Check if path is a directory
/// @param spath vt_str_t path or directory
/// @return true if is directory
static bool cync_tools_filter_is_dir(void *spath) {
    return vt_path_is_dir(vt_str_z(spath));
}

/// @brief Check if path is a directory
/// @param spath vt_str_t path or directory
/// @return true if is directory
static bool cync_tools_filter_is_file(void *spath) {
    return vt_path_is_file(vt_str_z(spath));
}


