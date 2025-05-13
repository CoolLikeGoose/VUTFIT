//
// Created by burluiva on 11/9/2023.
//

#include "logger.h"

void log_error(const char *func, const char *file, int line, const char *format,
               ...) {
  va_list args;
  va_start(args, format);

  fprintf(stderr, "%s[ERROR] (from func %s in file %s at line %d) - ", KRED,
          func, file, line);
  vfprintf(stderr, format, args);
  fprintf(stderr, "%s\n", KNRM);

  va_end(args);
}

void log_warn(const char *func, const char *file, int line, const char *format,
              ...) {
  va_list args;
  va_start(args, format);

  fprintf(stderr, "%s[WARN] (from func %s in file %s at line %d) - ", KYEL,
          func, file, line);
  vfprintf(stderr, format, args);
  fprintf(stderr, "%s\n", KNRM);

  va_end(args);
}

void log_info(const char *func, const char *file, int line, const char *format,
              ...) {
  va_list args;
  va_start(args, format);

  fprintf(stderr, "%s[INFO] (from func %s in file %s at line %d) - ", KGRN,
          func, file, line);
  vfprintf(stderr, format, args);
  fprintf(stderr, "%s\n", KNRM);

  va_end(args);
}

void log_info_white(const char *func, const char *file, int line, const char *format,
                    ...) {
  va_list args;
  va_start(args, format);

  fprintf(stderr, "%s[INFO] (from func %s in file %s at line %d) - ", KNRM,
          func, file, line);
  vfprintf(stderr, format, args);
  fprintf(stderr, "%s\n", KNRM);

  va_end(args);
}