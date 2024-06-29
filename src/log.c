#include "log.h"

void cync_log(const char *const msg) {
    printf("[ %s ] %s\n", CYNC_PROJECT_NAME, msg);
}

