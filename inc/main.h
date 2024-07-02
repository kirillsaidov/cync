#ifndef CYNC_MAIN
#define CYNC_MAIN

#include <stdio.h>
#include <stdlib.h>
#include "vita/vita.h"

// project constants
#define CYNC_PROJECT_NAME "cync"
#define CYNC_PROJECT_VERSION "v1.0.1"
#define CYNC_PROJECT_HELP_HEADER "cync " CYNC_PROJECT_VERSION " -- syncing files accross directories and devices."
#define CYNC_PROJECT_HELP_FOOTER "OPTIONS:\n\
\t--schema=0  master-slave syncronization (default)\n\
\t--schema=1  sync both targets, but don't delete items\n\
\t--schema=2  sync both targets completely\n\
EXAMPLE:\n\tcync --target1 ~/disk1 --target2 ~/disk2 --schema 0 --verbose\
"

// syncronization schema
enum CyncSchema {
    CYNC_SCHEMA_MASTER_SLAVE,   // master-slave syncronization
    CYNC_SCHEMA_DUAL_SYNC,      // sync both targets, but don't delete items
    CYNC_SCHEMA_FULL_SYNC,      // sync both targets completely
    CYNC_SCHEMA_LOCAL_NETWORK,  // sync both targets over network
    CYNC_SCHEMA_COUNT,          // number of elements
};

#endif // CYNC_MAIN

