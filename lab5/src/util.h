#pragma once

// Must be called prior to any 'dbg' calls.
void init_dbg();

// Prints debug message if debug level 'level' is enabled.
// To enable it, use environment variable MY_DEBUG=<level>.
// For example:
//   $ MY_DEBUG=3 arch.exe -decompress in.bin out.txt
// In this case dbg calls with the first argument 1 .. 3 will cause debug
// output.
void dbg(int level, const char *format, ...);

//
void  dbg_print_string_hex(int level, const char *str);

// Outputs an error message to stderr
void error(const char *format, ...);

// Outputs a fatal error message and terminates the process.
// Usage example:
// if (n != expected) {
//   fatal_error("invalid n: %d, must be %d", n, expected);
// }
void fatal_error(const char *format,...);

// Outputs a fatal system error message and terminates the process.
// In addition to fatal_error, prints the value of the 'errno' global system
// error state variable.
// Usage example:
// if (!(f = fopen(input_file_name, "rb")) {
//   fatal_sys_error("can't open file <%s>", input_file_name);
// }
void fatal_sys_error(const char *format,...);

// Checks the 'errno' global, then outputs an error message and terminates the
// process if it is not zero. 'errno' is set to an error code by C runtime
// system calls (such as I/O) in case of an error.
void check_errno(const char *format, ...);

//https://stackoverflow.com/questions/1598985/c-read-binary-stdin
#ifdef _WIN32
# include <io.h>
# include <fcntl.h>
# define SET_BINARY_MODE(handle) _setmode(_fileno(handle), O_BINARY)
#else
# define SET_BINARY_MODE(handle) ((void)0)
#endif