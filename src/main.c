#include "main.h"

int main(const int argc, const char *argv[]) {
    // options
    char *opt_target1 = NULL;
    char *opt_target2 = NULL;
    bool opt_verbose = false;
    enum CyncSchema opt_schema = CYNC_SCHEMA_MASTER_SLAVE;

    // setup argopt parser
    vt_argopt_t optv[] = {
        //  long        short     description               argument                type
        { "--target1",  "-t1",   "path to target folder 1", VT_ARGOPT(opt_target1), VT_TYPE_CSTR },
        { "--target2",  "-t2",   "path to target folder 2", VT_ARGOPT(opt_target2), VT_TYPE_CSTR },
        { "--verbose",  "-v",    "verbose output",          VT_ARGOPT(opt_verbose), VT_TYPE_BOOL },
        { "--schema",   "-s",    "syncronization schema",   VT_ARGOPT(opt_schema),  VT_TYPE_INT32 },
    };
    const size_t optc = sizeof(optv)/sizeof(vt_argopt_t);

    // parse args and opts
    const int8_t parse_status = argc > 1 ? vt_argopt_parse(argc, argv, optc, optv, NULL) : VT_ARGOPT_PARSE_HELP_WANTED;
    if (parse_status < 0) {
        printf("See 'argopt -h' for more info!\n");
        goto cleanup;
    }

    // display help manual
    if (parse_status == VT_ARGOPT_PARSE_HELP_WANTED) {
        vt_argopt_print_help(
            "cync v1.0.0 -- syncing files accross directories and devices.",          // header
            "Example: cync --target1 ~/disk1 --target2 ~/disk2 --schema 0 --verbose", // footer
            optc, optv
        );
        return 0;
    }

    // now do your thing
    printf("t1: %s\n", opt_target1);
    printf("t2: %s\n", opt_target1);
    printf(" v: %s\n", opt_verbose ? "true" : "false");
    printf(" s: %d\n", opt_schema);

cleanup:
    vt_free(opt_target1);
    vt_free(opt_target2);
    
    return 0;
}

