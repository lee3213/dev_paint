#pragma once
#include <string>
#include <stdarg.h>
std::string format(const char *fmt, ...);
std::string format_arg_list(const char *fmt, va_list args);