#pragma once

//https://stackoverflow.com/questions/1598985/c-read-binary-stdin
#ifdef _WIN32
# include <io.h>
# include <fcntl.h>
# define SET_BINARY_MODE(handle) _setmode(_fileno(handle), O_BINARY)
#else
# define SET_BINARY_MODE(handle) ((void)0)
#endif