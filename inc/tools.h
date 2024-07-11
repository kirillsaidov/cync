#ifndef CYNC_TOOLS
#define CYNC_TOOLS

#include "log.h"

// copy buffer size
#define CYNC_ONE_KB 1024
#define CYNC_ONE_MB (CYNC_ONE_KB * CYNC_ONE_KB)
#define CYNC_COPY_SIZE_LOW_MEM (CYNC_ONE_MB * 1)
#define CYNC_COPY_SIZE_DEFAULT (CYNC_ONE_MB * 16)
#define CYNC_COPY_SIZE_LARGE (CYNC_ONE_MB * 96)

/**
 * @brief  Filter list
 * @note   
 * @param  list: list of elements
 * @param  filter: filter function that accepts list item and returns a boolean if satisfies the filtering condition
 * @param  alloctr: alloctr or NULL to use vt_calloc/vt_free
 * @return filtered list or NULL if nothing satisfies the filtering condition
 */
extern vt_plist_t *cync_tools_filter_list(vt_plist_t *const list, bool (*filter)(void*), vt_mallocator_t *const alloctr);

/**
 * @brief  Copy file from source to destination
 * @note   
 * @param  src: src filepath
 * @param  dst: dst filepath
 * @param  low_mem: use less memory
 * @return true if copy success else false
 */
extern bool cync_tools_copy_file(const char *const src, const char *const dst, const bool low_mem);

/**
 * @brief  Returns file modification date
 * @note   
 * @param  filepath: path to file
 * @return time_t if success else 0
 */
extern time_t cync_tools_get_filetime_modified(const char *const filepath);

/**
 * @brief  Create directory tree
 * @note   
 * @param  src: source directory where the dir tree is copied from
 * @param  dst: destination directory where the dir tree is created based on source
 * @param  verbose: verbose output
 * @param  ignore_df: ignore dot files
 * @param  alloctr: alloctr or NULL to use vt_calloc/vt_free
 * @return None
 */
extern void cync_tools_create_dirtree(const char *const src, const char *const dst, const bool ignore_df, const bool verbose, vt_mallocator_t *const alloctr);

/**
 * @brief  Copy or update files in the destination directory
 * @note   
 * @param  src: source directory where files are taken from
 * @param  dst: destination directory where files are copied to
 * @param  verbose: verbose output
 * @param  low_mem: use less memory
 * @param  ignore_df: ignore dot files
 * @param  alloctr: alloctr or NULL to use vt_calloc/vt_free
 * @return None
 */
extern void cync_tools_copy_update_files(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose, vt_mallocator_t *const alloctr);

/**
 * @brief  Remove files from destination directory if they are not present in the source directory
 * @note   
 * @param  src: source directory where files are compared to destination
 * @param  dst: destination directory where files are removed if absent from source
 * @param  verbose: verbose output
 * @param  ignore_df: ignore dot files
 * @param  alloctr: alloctr or NULL to use vt_calloc/vt_free
 * @return None
 */
extern void cync_tools_remove_files(const char *const src, const char *const dst, const bool ignore_df, const bool verbose, vt_mallocator_t *const alloctr);

/**
 * @brief  Remove directory tree from destination if it is not present in the source directory
 * @note   
 * @param  src: source directory where the dir tree is compared to destination
 * @param  dst: destination directory where the dir tree is removed if absent from source
 * @param  verbose: verbose output
 * @param  ignore_df: ignore dot files
 * @param  alloctr: alloctr or NULL to use vt_calloc/vt_free
 * @return None
 */
extern void cync_tools_remove_dirtree(const char *const src, const char *const dst, const bool ignore_df, const bool verbose, vt_mallocator_t *const alloctr);

#endif // CYNC_TOOLS

