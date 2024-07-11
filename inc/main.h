#ifndef CYNC_MAIN
#define CYNC_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vita/vita.h"

// project constants
#define CYNC_PROJECT_NAME "cync"
#define CYNC_PROJECT_VERSION "v1.0.3"
#define CYNC_PROJECT_HELP_HEADER "cync " CYNC_PROJECT_VERSION " -- syncing files accross directories and devices."
#define CYNC_PROJECT_HELP_FOOTER "OPTIONS:\n\
\t--mode=0  ensure the destination folder is a strict copy of the source specified (default)\n\
\t--mode=1  synchronize both targets, but do not remove files automatically\n\
\t--mode=2  synchronize both targets completely (fully automatic)\n\
\t--mode=3  synchronize both targets over the network in auto mode\n\
EXAMPLE:\n\tcync --src ~/disk1 --dst ~/disk2 --mode 0 --verbose\
"

// syncronization mode
enum CyncMode {
    CYNC_MODE_TARGET, // ensure the destination folder is a strict copy of the source specified
    CYNC_MODE_DUAL,   // synchronize both targets, but do not remove files automatically
    CYNC_MODE_AUTO,   // synchronize both targets completely (fully automatic)
    CYNC_MODE_NET,    // synchronize both targets over the network in auto mode
    CYNC_MODE_COUNT,  // number of elements
};

extern vt_mallocator_t *alloctr;

#endif // CYNC_MAIN

