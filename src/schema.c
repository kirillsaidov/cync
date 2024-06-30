#include "schema.h"

// copy buffer size
#define CYNC_ONE_KB 1024
#define CYNC_ONE_MB CYNC_ONE_KB * 1000

/**
 * @brief  Find string in list
 * @note   
 * @param  s: string
 * @param  list: list of strings
 * @param  method: 0: equals s==list(str), 1: can find s in list(str), 2: list(str) ends with s
 * @retval true if str in list else false
 */
static bool cync_find_str_in_list(const vt_str_t *const s, vt_plist_t *const list, const int method);

/**
 * @brief  Copy file from source to destination
 * @note   
 * @param  src: src filepath
 * @param  dst: dst filepath
 * @retval true if copy success else false
 */
static bool cync_copy_file(const char *const src, const char *const dst);

/**
 * @brief  Filter all directories
 * @note   
 * @param  list: list of all directory contents
 * @retval list of directories
 */
static vt_plist_t *cync_filter_dirs(vt_plist_t *const list);

/**
 * @brief  Returns file modification date
 * @note   
 * @param  filepath: path to file
 * @retval time_t if success else 0
 */
static time_t cync_file_time_modified(const char *const filepath);


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
    printf("listdir start.\n");
    vt_plist_t *dir_master = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    vt_plist_t *dir_slave = vt_plist_create(VT_ARRAY_DEFAULT_INIT_ELEMENTS, alloctr);
    dir_master = vt_path_dir_list_recurse(dir_master, master, ignore_df);
    dir_slave = vt_path_dir_list_recurse(dir_slave, slave, ignore_df);

    printf("listdir done.\n");

    // create slave dirtree if does not exist
    void *path = NULL;
    vt_plist_t *dirs = cync_filter_dirs(dir_master);
    while ((path = vt_plist_slide_front(dirs)) != NULL) {
        vt_str_t *item = path;
        
        // replace master dirtree with slave dirtree
        vt_str_replace(item, vt_str_z(path_master), vt_str_z(path_slave));

        // create dirtree in slave
        if (!vt_path_exists(vt_str_z(item))) {
            const bool ret = vt_path_mkdir_parents(vt_str_z(item));
            if (verbose) cync_log_ln("Creating directory <%s> %s.", vt_str_z(item), ret ? "SUCCESS" : "FAILED");
        }
    }

    printf("createdir done.\n");
    
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
        vt_str_replace(slave_file, vt_str_z(path_master), vt_str_z(path_slave));

        // check if item exists in slave
        if (cync_find_str_in_list(basename, dir_slave, 2)) {
            // get modification date
            const time_t modiftime_master = cync_file_time_modified(vt_str_z(item));
            const time_t modiftime_slave = cync_file_time_modified(vt_str_z(slave_file));
            
            // copy if needed
            if (modiftime_master > modiftime_slave) {
                // copy file
                const bool ret = cync_copy_file(vt_str_z(item), vt_str_z(slave_file));
                if (verbose) cync_log_ln("Update file <%s> %s.", vt_str_z(slave_file), ret ? "SUCCESS" : "FAILED");
            }
        } else {
            // skip if file exists
            if (vt_path_exists(vt_str_z(slave_file))) continue;

            // copy file
            if (verbose) cync_log_ln("Copying file <%s>", vt_str_z(item));
            const bool ret = cync_copy_file(vt_str_z(item), vt_str_z(slave_file));
            if (verbose) cync_log_ln("To <%s> %s.", vt_str_z(slave_file), ret ? "SUCCESS" : "FAILED");
        }
    }

    // free resources
    vt_mallocator_destroy(alloctr);
    if (verbose) cync_log_ln("All files up-to-date.");
}


/** 
 * ------------------- PRIVATE -------------------
 */

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

static bool cync_copy_file(const char *const src, const char *const dst) {
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

static vt_plist_t *cync_filter_dirs(vt_plist_t *const list) {
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

static time_t cync_file_time_modified(const char *const filepath) {
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

