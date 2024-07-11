#ifndef CYNC_MODE
#define CYNC_MODE

#include "main.h"
#include "log.h"
#include "tools.h"

/**
 * @brief  Ensure the destination folder is a strict copy of the source specified
 * @note   
 * @param  src: source folder
 * @param  dst: destination folder
 * @param  ignore_df: ignore dot files
 * @param  low_mem: use less memory
 * @param  verbose: verbose output
 * @return None
 */
extern void cync_mode_target(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose);

/**
 * @brief  Synchronize both targets, but do not remove files automatically
 * @note   
 * @param  src: source folder
 * @param  dst: destination folder
 * @param  ignore_df: ignore dot files
 * @param  low_mem: use less memory
 * @param  verbose: verbose output
 * @return None
 */
extern void cync_mode_dual(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose);

/**
 * @brief  Synchronize both targets completely (fully automatic)
 * @note   
 * @param  src: source folder
 * @param  dst: destination folder
 * @param  ignore_df: ignore dot files
 * @param  low_mem: use less memory
 * @param  verbose: verbose output
 * @return None
 */
extern void cync_mode_auto(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose);

/**
 * @brief  Synchronize both targets over the network in auto mode
 * @note   
 * @param  src: source folder
 * @param  dst: destination folder
 * @param  ignore_df: ignore dot files
 * @param  low_mem: use less memory
 * @param  verbose: verbose output
 * @return None
 */
extern void cync_mode_net(const char *const src, const char *const dst, const bool ignore_df, const bool low_mem, const bool verbose);

#endif // CYNC_MODE

