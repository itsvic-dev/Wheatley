#include "string.h"
#include <stddef.h>

size_t strlen(const char *str) {
  const char *s = str;
  for (s = str; *s; s++)
    ;
  return s - str;
}

char *strcpy(char *dest, const char *source) {
  memcpy(dest, source, strlen(source));
  return dest;
}
