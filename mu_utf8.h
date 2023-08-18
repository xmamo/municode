#ifndef MU_UTF8_H
#define MU_UTF8_H

#include <stdint.h>
#include <stddef.h>

typedef uint_least32_t char32_t;

char32_t mu_utf8_next(const char** utf8);

size_t mu_cp_utf8_length(char32_t cp);

#define mu_utf8_next(utf8) _Generic((utf8),   \
  char**: (char32_t (*)(char**))mu_utf8_next, \
  default: mu_utf8_next                       \
)(utf8)

#endif
