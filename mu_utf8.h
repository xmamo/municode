#ifndef MU_UTF8_H
#define MU_UTF8_H

#include <stddef.h>
#include <stdint.h>

typedef uint_least32_t char32_t;

char32_t mu_utf8_next(const char* utf8, size_t* i);

size_t mu_cp_utf8_length(char32_t cp);

#endif
