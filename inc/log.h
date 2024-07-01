#ifndef CYNC_LOG
#define CYNC_LOG

#include "main.h"

/**
 * @brief  Log text to stdout
 * @note   
 * @param  *format: format or text
 * @retval None
 */
extern void cync_log(const char *format, ...);

/**
 * @brief  Log text to stdout with new line appended at the end
 * @note   
 * @param  *format: format or text
 * @retval None
 */
extern void cync_log_ln(const char *format, ...);

#endif // CYNC_LOG

