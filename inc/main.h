#ifndef CYNC_MAIN
#define CYNC_MAIN

#include <stdio.h>
#include <stdlib.h>
#include "vita/vita.h"

// ditto
#define CYNC_PROJECT_NAME "cync"

// syncronization schema
enum CyncSchema {
    CYNC_SCHEMA_MASTER_SLAVE,   // master-slave relationship
    CYNC_SCHEMA_DUAL_SYNC,      // sync both targets, but don't delete items
    CYNC_SCHEMA_FULL_SYNC,      // sync both targets completely
    CYNC_SCHEMA_LOCAL_NETWORK,  // sync both targets over network
    CYNC_SCHEMA_COUNT,          // number of elements
};

#endif // CYNC_MAIN