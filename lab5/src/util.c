#include "util.h"

#include <errno.h>  // errno,...
#include <stdarg.h> // va_start(),...
#include <stdbool.h> // to enable bool type
#include <stdio.h>  // fprintf(),...
#include <stdlib.h> // exit(),...
#include <string.h>  // strerror(),...

// TODO factor out common code in *error functions

void fatal_error(const char *format, ...) {
	// declare uninitialized argument list
	va_list args;
	// bind it to the first argument
	va_start(args, format);
	// print the error tag
	fprintf(stderr, "*** ERROR: ");
	// print the message, passing through the variable argument list
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	// notify the C runtime that argument are handled before return 
	va_end(args);
	exit(1);
}

void error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	fprintf(stderr, "*** ERROR: ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
}

void fatal_sys_error(const char *format, ...) {
	va_list args;
	va_start(args, format);
	fprintf(stderr, "*** SYSTEM ERROR (%s):\n", strerror(errno));
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}

void check_errno(const char *format, ...) {
    va_list args;
    va_start(args, format);
    bool err = false;

    if (errno) {
        err = true;
        fprintf(stderr, "*** SYSTEM ERROR (%s):\n", strerror(errno));
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
    }
    va_end(args);

    if (err) {
        exit(1);
    }
}

////////// Debug printing

// 'static' makes debug_level inaccessible from other source files
static int debug_level = 0;

void init_dbg() {
    const char *s = getenv("MY_DEBUG");
    if (!s) {
        return;
    }
    debug_level = atoi(s);
    dbg(1, "debug level set to %d", debug_level);
}

void dbg(int level, const char *format, ...) {
    va_list args;
    va_start(args, format);
    if (debug_level == 0 || level > debug_level) {
        va_end(args);
        return;
    }
    fprintf(stderr, ">>> DEBUG: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void  dbg_print_string_hex(int level, const char *str) {
    if (debug_level == 0 || level > debug_level) {
        return;
    }
    int str_size = strlen(str);

    for (int i = 0; i < str_size; i++) {
        fprintf(stderr, "%02x", str[i]);
    }
}