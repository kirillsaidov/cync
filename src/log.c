#include "log.h"

void cync_log(const char *format, ...) {
    va_list args;
    va_start(args, format); 
    {
        // print
        printf("[ %s ] ", CYNC_PROJECT_NAME);
        vprintf(format, args);
    }
    va_end(args);
}

void cync_log_ln(const char *format, ...) {
    va_list args;
    va_start(args, format); 
    {
        // print
        printf("[ %s ] ", CYNC_PROJECT_NAME);
        vprintf(format, args);
        printf("\n");
    }
    va_end(args);
}

