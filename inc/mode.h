#ifndef CYNC_MODE
#define CYNC_MODE

#include "main.h"
#include "log.h"
#include "tools.h"

/**
 * @brief  Master-slave syncronization approach
 * @note   
 * @param  master: source folder
 * @param  slave: destination folder
 * @param  ignore_df: ignore dot files
 * @param  low_mem: use less memory
 * @param  verbose: verbose output
 * @retval None
 */
extern void cync_schema_master_slave(const char *const master, const char *const slave, const bool ignore_df, const bool low_mem, const bool verbose);

#endif // CYNC_MODE

