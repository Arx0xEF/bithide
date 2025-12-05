#include "include/steg.h"

typedef enum arg_mode {
    MODE_HELP = 0,
    MODE_HEADER,
    MODE_EMBED,
    MODE_EXTRACT,
    MODE_VERBOSE,
    MODE_VERSION,
    MODE_F_INFO
} arg_mode;

typedef enum ext_mode {
    STEGOFILE,
    PASSPHRASE,
    EXTRACT_FILE
} ext_mode;

typedef enum emd_mode {
    EMBED_FILE,
    COVER_FILE,
    PASSPHRASE,
    // STEGOFILE,
} emd_mode;

typedef struct Config {
    enum arg_mode mode;
    unsigned int argcount;
    char *cover_file;
    char *secret_file;
    // char *embed_opt;
    // char *extract_opt;
    char *pass_phrase;
} Config;

Config parse_args(int argc, char **argv) {
    Config opt = {0};
    opt.mode = MODE_HELP;
    if(argc < 2)
        return opt;
    for(int i = 1; i < argc; i++) {
        if(!strcmp(argv[1], "--info") || !strcmp(argv[1], "--h")) {
            opt.mode = MODE_HEADER;
        }
        else if(!strcmp(argv[1], "embed") || !strcmp(argv[1], "--embed")) {
            opt.mode = MODE_EMBED;
        }
        else if(!strcmp(argv[1], "extract") || !strcmp(argv[1], "--extract")) {
            opt.mode = MODE_EXTRACT;
        }
        else if(!strcmp(argv[1], "version") || !strcmp(argv[1], "--version")) {
            opt.mode = MODE_VERSION;
        }
        else if(!strcmp(argv[1], "--verbose") || !strcmp(argv[1], "-v")) {
            opt.mode = MODE_VERBOSE;
        }
    }
    return opt;
}
