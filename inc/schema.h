#ifndef CYNC_SCHEMA
#define CYNC_SCHEMA

#include "main.h"

/**
 * @brief  Master-slave syncronization approach
 * @note   
 * @param  master: target folder 1
 * @param  slave: target folder 2
 * @param  verbose: verbose output
 * @retval None
 */
void cync_schema_master_slave(const char *const master, const char *const slave, const bool verbose);


#endif // CYNC_SCHEMA