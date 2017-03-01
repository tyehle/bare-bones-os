#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
  const unsigned char* bytes = (const unsigned char*) data;
  for(size_t i = 0; i < length; i++) {
    if(putchar(bytes[i]) == EOF) return false;
    else continue;
  }
  return true;
}

int printf(const char* restrict format, ...) {
  va_list parameters;
  va_start(parameters, format);

  int written = 0;

  while(format[0] != '\0') {
    size_t maxrem = INT_MAX - written;

    if(format[0] != '%' || format[1] == '%') {
      if(format[0] == '%') format += 1;

      // find how much of the formatstring we can write
      size_t amount = 1;
      while(format[amount] != '\0' && format[amount] != '%') amount += 1;

      if(maxrem < amount) {
        // TODO: set errno to EOVERFLOW
        return -1;
      } else if(!print(format, amount)) {
        return -1;
      } else {
        format += amount;
        written += amount;
      }
    } else {
      const char* format_begun_at = format;
      format += 1;

      if(format[0] == 'c') {
        format += 1;
        char c = (char) va_arg(parameters, int); // char gets promoted to int
        if(maxrem == 0) {
          // TODO: set errno to EOVERFLOW
          return -1;
        } else if (!print(&c, sizeof(c))) {
          return -1;
        } else {
          written += 1;
        }
      } else if(format[0] == 's') {
        format += 1;
        const char* str = (const char*) va_arg(parameters, const char*);
        size_t length = strlen(str);
        if(maxrem < length) {
          // TODO: set errno to EOVERFLOW
          return -1;
        } else if(!print(str, length)) {
          return -1;
        } else {
          written += length;
        }
      } else {
        // This is undefined behavior in C99
        // safest bet is probably just to print the rest of the format string
        format = format_begun_at;
        size_t length = strlen(format);
        if(maxrem < length) {
          // TODO: set errno to EOVERFLOW
          return -1;
        } else if(!print(format, length)) {
          return -1;
        } else {
          written += length;
          format += length;
        }
      }
    }
  }

  va_end(parameters);
  return written;
}
