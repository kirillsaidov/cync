#ifndef CYNC_SYNC_TOOLS
#define CYNC_SYNC_TOOLS

#include "main.h"
#include "log.h"

// copy buffer size
#define CYNC_ONE_KB 1024
#define CYNC_ONE_MB (CYNC_ONE_KB * CYNC_ONE_KB)
#define CYNC_COPY_SIZE_LOW_MEM (CYNC_ONE_MB * 1)
#define CYNC_COPY_SIZE_DEFAULT (CYNC_ONE_MB * 10)
#define CYNC_COPY_SIZE_LARGE (CYNC_ONE_MB * 100)

/**
 * @brief  Find string in list
 * @note   
 * @param  s: string
 * @param  list: list of strings
 * @param  method: 0: equals s==list(str), 1: can find s in list(str), 2: list(str) ends with s
 * @retval true if str in list else false
 */
extern bool cync_find_str_in_list(const vt_str_t *const s, vt_plist_t *const list, const int method);

/**
 * @brief  Copy file from source to destination
 * @note   
 * @param  src: src filepath
 * @param  dst: dst filepath
 * @param  low_mem: use less memory
 * @retval true if copy success else false
 */
extern bool cync_copy_file(const char *const src, const char *const dst, const bool low_mem);

/**
 * @brief  Filter all directories
 * @note   
 * @param  list: list of all directory contents
 * @retval list of directories
 */
extern vt_plist_t *cync_filter_dirs(vt_plist_t *const list);

/**
 * @brief  Returns file modification date
 * @note   
 * @param  filepath: path to file
 * @retval time_t if success else 0
 */
extern time_t cync_file_time_modified(const char *const filepath);

#endif // CYNC_SYNC_TOOLS

