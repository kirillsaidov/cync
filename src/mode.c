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

void cync_mode_dual(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose) {
    VT_DEBUG_ASSERT(src != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    VT_DEBUG_ASSERT(dst != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    
    /**
     * Synchronize source, but do not remove files
     */

    // create directory tree for dst
    cync_tools_create_dirtree(src, dst, ignore_df, verbose, alloctr);

    // copy or update files from src to dst
    cync_tools_copy_update_files(src, dst, ignore_df, low_mem, verbose, alloctr);

    /**
     * Synchronize destination, but do not remove files
     */

    // create directory tree for dst
    cync_tools_create_dirtree(dst, src, ignore_df, verbose, alloctr);

    // copy or update files from src to dst
    cync_tools_copy_update_files(dst, src, ignore_df, low_mem, verbose, alloctr);
}

/*
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

