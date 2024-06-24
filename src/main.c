#include "main.h"
#include "vita/vita.h"

int32_t main(void) {
    vt_version_t v = vt_version_get();
    printf("Hello, CMake project template! | Vita %s\n", v.str);
    return 0;
}

