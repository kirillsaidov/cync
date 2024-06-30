#ifndef CYNC_SCHEMA
#define CYNC_SCHEMA

#include "main.h"
#include "log.h"

/**
 * @brief  Master-slave syncronization approach
 * @note   
 * @param  master: target folder 1
 * @param  slave: target folder 2
 * @param  ignore_df: ignore dot files
 * @param  verbose: verbose output
 * @retval None
 */
void cync_schema_master_slave(const char *const master, const char *const slave, const bool ignore_df, const bool verbose);

#endif // CYNC_SCHEMA