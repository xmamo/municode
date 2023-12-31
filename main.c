#include <assert.h>
#include <stdlib.h>

#include "mu.h"
#include <stdio.h>

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  static const struct {
    const char* string;
    char32_t cp;
  } TABLE[] = {
    {"\x4D", 0x4D},
    {"\xD0\xB0", 0x430},
    {"\xE4\xBA\x8C", 0x4E8C},
    {"\xF0\x90\x8C\x82", 0x10302},
    {"\xC0\xAF", -1},
    {"\xE0\x9F\x80", -1},
  };

  for (size_t i = 0; i < sizeof(TABLE) / sizeof(*TABLE); ++i) {
    size_t j = 0;
    assert(mu_utf8_next(TABLE[i].string, &j) == TABLE[i].cp);
  }

  assert(mu_cp_is_uppercase('A'));
  assert(mu_cp_is_lowercase('a'));

  return EXIT_SUCCESS;
}
