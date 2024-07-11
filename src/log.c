#include "log.h"

void cync_log(const char *format, ...) {
    VT_DEBUG_ASSERT(format != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    
    // parse args and print
    va_list args;
    va_start(args, format); 
    {
        printf("%s >> ", CYNC_PROJECT_NAME);
        vprintf(format, args);
    }
    va_end(args);
}

void cync_log_ln(const char *format, ...) {
    VT_DEBUG_ASSERT(format != NULL, "%s\n", vt_status_to_str(VT_STATUS_ERROR_INVALID_ARGUMENTS));
    
    // parse args and print
    va_list args;
    va_start(args, format); 
    {
        printf("%s >> ", CYNC_PROJECT_NAME);
        vprintf(format, args);
        printf("\n");
    }
    va_end(args);
}

