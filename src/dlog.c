#ifndef NDEBUG

#include "dlog.h"
#include <stdarg.h>
#include <stdio.h>

void dlog_impl(const char *func, int line, const char *fmt, ...) {
  va_list args;

  fprintf(stderr, "[DEBUG][%s:%d] ", func, line);

  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  fprintf(stderr, "\n");
}

#endif /* NDEBUG */
