#include "archiver.h"
#include "decompressor.h"
#include "util.h"
#include "copy_file_data.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// Convenience enum type to express tool's mode of operation
typedef enum {
    MODE_normal,
    MODE_stream,
    MODE_debug
} ToolMode;

// Convenience enum type to express tool's action
typedef enum {
    ACT_none,  // action not determined
    ACT_compress,
    ACT_decompress
} ToolAction;

// '##' concatenates scanner tokens
// '#' wraps argument into double quotes
#define CASE(x) case MODE_##x: return #x
const char* mode_to_str(ToolMode m) {
    switch (m) {
        CASE(normal);
        CASE(stream);
        CASE(debug);
    default:
        assert(0);
    }
    return "<unknown>";
}
#undef CASE

#define CASE(x) case ACT_##x: return #x
const char* act_to_str(ToolAction m) {
    switch (m) {
        CASE(none);
        CASE(compress);
        CASE(decompress);
    default:
        assert(0);
    }
    return "<unknown>";
}
#undef CASE

void usage() {
    fprintf(stderr,
        "Compressor/decompressor tool. The tool supports 3 modes of execution:    \n"
        "-- normal                                                                \n"
        "  input and output files are specified on the command line along with    \n"
        "  the archiver working format - compress or decompress:                  \n"
        "    $ arch.exe <op> <input file name> <output file name>                 \n"
        "  where <op> can be                                                      \n"
        "    -compress - compress the input into the output                       \n"
        "    -decompress - decompress the input into the output                   \n"
        "  ** 3 command line arguments are expected                               \n"
        "-- stream                                                                \n"
        "  input is stdin, output is stdout, the archiver working format set      \n"
        "  by the first line in stdin:                                            \n"
        "    c - 'compress'                                                       \n"
        "    d - 'decompress'                                                     \n"
        "  the first line must be terminated with '\\n' on Linux or '\\r' '\\n' on\n"
        "  Windows.                                                               \n"
        "    $ cat input.bin | arch.exe > out.bin                                 \n"
        "  ** no arguments are expected on the command line                       \n"
        "-- debug                                                                 \n"
        "  reopens input and output files under stdin and stdout handlers         \n"
        "  to model \"stream mode\" when debugging under IDE                      \n"
        "    $ arch.exe <input file name> <output file name> -dbg                 \n"
        "  ** 3 command line arguments are expected                               \n"
    );
}

#define ON_EXIT()       \
    if (input) {        \
        fclose(input);  \
    }                   \
    if (output) {       \
        fclose(output); \
    }

// correct reading of action format and line translation
ToolAction read_action(FILE *input) {
    // determine action by the first line of the input stream
    unsigned char ch;
    ToolAction act = ACT_none;
    if (!fread(&ch, 1, sizeof(ch), input)) {
        printf("can't read a command");
        exit(1);
    }
    check_errno("failed to read command");

    switch (ch) {
        case 'c':
            act = ACT_compress;
            break;
        case 'd':
            act = ACT_decompress;
            break;
        default:
            ; // error - handle it below
    }
    if (act == ACT_none) {
        fclose(input);
        fatal_error("invalid first line in the input");
    }
    // check there is a line terminator after the command
    if (!fread(&ch, 1, 1, input)) {
        printf("error reading\n");
        exit(1);
    }
    if (ch == '\r') {
        if (!fread(&ch, 1, 1, input)) {     // read next byte ('\n' - expected)
            printf("error reading\n");
            exit(1);
        }
    }
    if (ch != '\n') {
        fatal_error("bad line terminator"); // expected - '\n'
    }
    return act;
}

// read binary data from stdin and write it in the file
FILE *read_to_file_and_reopen(FILE *input, const char *fname) {
    FILE *output = fopen(fname, "wb");
    if (!output) {
        fatal_sys_error("can't open %s for writing", fname);    // error checker (forget it)
    }
    char buffer[129];
    const size_t max_len = sizeof(buffer) - 1;
    size_t n;

    while ((n = fread(buffer, 1, max_len, input)) != 0)
    {
        buffer[n] = '\0';
        dbg_print_string_hex(9, buffer); // added simple debug (forget it)
        dbg(9, "\n");             //
        fwrite(buffer, 1, n, output);
    }
    //fclose(input);    // no need to close stdin
    fclose(output);
    check_errno("error closing %s", fname); // error checker (forget it)
    input = fopen(fname, "rb");

    if (!input) {
        fatal_sys_error("can't open %s for reading", fname); // error checker (forget it)
    }
    return input;
}

int main(int argc, char *argv[]) {      // argc - size of input argv array
    init_dbg();
    ToolAction act = ACT_none;
    ToolMode mode = MODE_stream; // default, used if no arguments
    const char *in_file = NULL, *out_file = NULL;
    const char *temp_input_fname = "in_data.tmp";
    const char *temp_output_fname = "out_data.tmp";

    // parse command line arguments
    for (int argno = 1; argno < argc; ++argno) {
        const char *arg = argv[argno];

        if (!strcmp("-dbg", arg)) {
            if (mode != MODE_stream) {
                error("%s already specified", arg);
                usage();
                exit(1);
            }
            mode = MODE_debug;
            continue;
        }
        if (!strcmp("-compress", arg)) {
            // ignore multiple occurences of -compress or -decompress on the
            // command line and only use the last one
            act = ACT_compress;
            continue;
        }
        if (!strcmp("-decompress", arg)) {
            act = ACT_decompress;
            continue;
        }
        if (arg[0] == '-') {
            error("unknown option: %s", arg);
            usage();
            exit(1);
        }
        // the argument is not an option, so it can only be either in_file or out_file
        // file
        if (in_file && out_file) {
            // both already initialized, which means this is the third occurrence of a
            // non-option argument - invalid usage
            error("unexpected argument: %s", arg);
            usage();
            exit(1);
        }
        // initialize in_file if it is still NULL otherwise initialize out_file
        if (!in_file) {
            in_file = arg;
            continue;
        }
        assert(!out_file && "out_file must be NULL here");
        out_file = arg;
    }
    // perform error checking and in_file/out_file initialization
    if (mode == MODE_debug && act != ACT_none) {
        error("action can't be specified together with -dbg");
        usage();
        exit(1);
    }
    FILE *input = NULL, *output = NULL;

    if (act != ACT_none) {
        // don't override debug mode, change only if it is not
        mode = MODE_normal; // -compressed/-decompress specified - normal mode
    }
    switch (mode) {
    case MODE_stream:
        if (in_file || out_file) { // action not set
            usage();
            exit(1);
        }
        SET_BINARY_MODE(stdin);     // set mode on stdin for binary reading
        SET_BINARY_MODE(stdout);    //  set mode on stdout for binary writing
        act = read_action(stdin);
        input = read_to_file_and_reopen(stdin, temp_input_fname);

        if (!(output = fopen(temp_output_fname, "wb"))) {
            ON_EXIT(); // cleanup before exiting
            fatal_sys_error("can't open %s for writing", temp_output_fname);
        }
        break;
    case MODE_normal:
        if (!(input = fopen(in_file, "rb"))) {
            fatal_sys_error("can't open %s for reading", in_file);
        }
        if (!(output = fopen(out_file, "wb"))) {
            ON_EXIT(); // cleanup before exiting
            fatal_sys_error("can't open %s for writing", out_file);
        }
        break;
    default:
        fprintf(stderr, "bad mode\n");
    }

    dbg(1, "action=%s, mode=%s", act_to_str(act), mode_to_str(mode));
    if (act == ACT_compress) {
        archive(input, output);
    }
    else {
        assert(act == ACT_decompress);
        decompress(input, output);
    }
    fclose(input);
    fclose(output);

    if (mode == MODE_stream) {
        if (!(output = fopen(temp_output_fname, "rb"))) {
            fatal_sys_error("can't open %s for reading", temp_output_fname);
        }
        copy_data(output, stdout);

        fclose(output);
        remove(temp_input_fname);   // remove files with stdin and stdout data
        remove(temp_output_fname);
    }
    return 0;
}
