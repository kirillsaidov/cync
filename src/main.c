#include "main.h"
#include "log.h"
#include "schema.h"
#include "sync_tools.h"

int main(const int argc, const char *argv[]) {
    // options
    char *opt_target1 = NULL;
    char *opt_target2 = NULL;
    bool opt_verbose = false;
    bool opt_move_df = false;
    enum CyncSchema opt_schema = CYNC_SCHEMA_MASTER_SLAVE;

    // setup argopt parser
    vt_argopt_t optv[] = {
        //  long            short     description               argument                    type
        { "--target1",      "-t1",   "path to target folder 1", VT_ARGOPT(opt_target1),     VT_TYPE_CSTR },
        { "--target2",      "-t2",   "path to target folder 2", VT_ARGOPT(opt_target2),     VT_TYPE_CSTR },
        { "--verbose",      "-v",    "verbose output",          VT_ARGOPT(opt_verbose),     VT_TYPE_BOOL },
        { "--move_df",      "-m",    "move dot files",          VT_ARGOPT(opt_move_df),     VT_TYPE_BOOL },
        { "--schema",       "-s",    "syncronization schema",   VT_ARGOPT(opt_schema),      VT_TYPE_INT32 },
    };
    const size_t optc = sizeof(optv)/sizeof(vt_argopt_t);

    // parse args and opts
    const int8_t parse_status = argc > 1 ? vt_argopt_parse(argc, argv, optc, optv, NULL) : VT_ARGOPT_PARSE_HELP_WANTED;
    if (parse_status < 0) {
        printf("See 'cync -h' for more info!\n");
        goto cleanup;
    }

    // display help manual
    if (parse_status == VT_ARGOPT_PARSE_HELP_WANTED) {
        vt_argopt_print_help(
            CYNC_PROJECT_HELP_HEADER, // header
            CYNC_PROJECT_HELP_FOOTER, // footer
            optc, optv
        );
        return 0;
    }

    // check conditions
    if (!opt_target1) {
        cync_log("Source directory wasn't specified!");
        goto cleanup;
    }
    if (!opt_target2) {
        cync_log("Destination directory wasn't specified!");
        goto cleanup;
    }
    if (opt_schema > 0 && opt_schema < CYNC_SCHEMA_COUNT) {
        cync_log("Unknown syncronization schema specified!");
        goto cleanup;
    }

    // select syncronization approach and sync data
    switch (opt_schema) {
        case CYNC_SCHEMA_MASTER_SLAVE: 
            cync_schema_master_slave(opt_target1, opt_target2, !opt_move_df, opt_verbose);
            break;
        case CYNC_SCHEMA_DUAL_SYNC:
        case CYNC_SCHEMA_FULL_SYNC:
        case CYNC_SCHEMA_LOCAL_NETWORK:
            cync_log("Feature unimplemented!");
            break;
        default: cync_log("Unknown or unsupported schema!");
    }

cleanup:
    vt_free(opt_target1);
    vt_free(opt_target2);

    return 0;
}

