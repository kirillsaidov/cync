#include "main.h"
#include "log.h"
#include "mode.h"
#include "tools.h"

vt_mallocator_t *alloctr = NULL;

int main(const int argc, const char *argv[]) {
    if (argc < 2) {
        cync_log_ln("No commands specified! See '-h' for more information.");
        return 0;
    }

    // create allocator
    alloctr = vt_mallocator_create();

    // define options
    char *opt_src = NULL;
    char *opt_dst = NULL;
    bool opt_verbose = false;
    bool opt_low_mem = false;
    bool opt_ignore_df = false;
    enum CyncMode opt_mode = CYNC_MODE_TARGET;

    // setup argopt parser
    vt_argopt_t optv[] = {
        //  long            short     description               argument                     type
        { "--src",          "-s",   "source directory",         VT_ARGOPT(opt_src),          VT_TYPE_CSTR },
        { "--dst",          "-d",   "destination directory",    VT_ARGOPT(opt_dst),          VT_TYPE_CSTR },
        { "--verbose",      "-v",   "verbose output",           VT_ARGOPT(opt_verbose),      VT_TYPE_BOOL },
        { "--ignore_df",    "-i",   "ignore dot files",         VT_ARGOPT(opt_ignore_df),    VT_TYPE_BOOL },
        { "--low_mem",      "-l",   "use less memory",          VT_ARGOPT(opt_low_mem),      VT_TYPE_BOOL },
        { "--mode",         "-m",   "syncronization mode",      VT_ARGOPT(opt_mode),         VT_TYPE_INT32 },
    };
    const size_t optc = sizeof(optv)/sizeof(vt_argopt_t);

    // parse args and opts
    const int8_t parse_status = vt_argopt_parse(argc, argv, optc, optv, NULL);
    if (parse_status < 0) {
        cync_log_ln("See '-h' for more information!");
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
    if (!opt_src) {
        cync_log_ln("Source directory wasn't specified!");
        goto cleanup;
    }
    if (!opt_dst) {
        cync_log_ln("Destination directory wasn't specified!");
        goto cleanup;
    }
    if (!vt_path_exists(opt_src)) {
        cync_log_ln("Source directory does not exist <%s>!", opt_src);
        goto cleanup;
    }
    if (!vt_path_exists(opt_dst)) {
        cync_log_ln("Destination directory does not exist <%s>!", opt_dst);
        goto cleanup;
    }
    if (vt_str_equals_z(opt_src, opt_dst)) {
        cync_log_ln("Source and destination directories cannot be the same!");
        goto cleanup;
    }
    if (!(opt_mode >= 0 && opt_mode < CYNC_MODE_COUNT)) {
        cync_log_ln("Unknown syncronization mode <%d> specified!", opt_mode);
        goto cleanup;
    }

    // select syncronization approach and sync data
    if (opt_verbose) cync_log_ln("Starting syncronization process...");
    switch (opt_mode) {
        case CYNC_MODE_TARGET: 
            cync_mode_target(opt_src, opt_dst, opt_ignore_df, opt_low_mem, opt_verbose);
            break;
        case CYNC_MODE_DUAL:
            cync_mode_dual(opt_src, opt_dst, opt_ignore_df, opt_low_mem, opt_verbose);
            break; 
        case CYNC_MODE_AUTO:
        case CYNC_MODE_NET:
            cync_log_ln("Feature unimplemented!");
            break;
        default:;
    }
    if (opt_verbose) cync_log_ln("All files up-to-date.");

cleanup:
    VT_FREE(opt_src);
    VT_FREE(opt_dst);
    vt_mallocator_destroy(alloctr);

    return 0;
}

