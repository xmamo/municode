#include <assert.h>
#include <stdlib.h>

#include "unicode.h"

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  static const struct {
    const char* string;
    char32_t cp;
  } table[] = {
    {"\x4D", 0x4D},
    {"\xD0\xB0", 0x430},
    {"\xE4\xBA\x8C", 0x4E8C},
    {"\xF0\x90\x8C\x82", 0x10302},
    {"\xC0\xAF", -1},
    {"\xE0\x9F\x80", -1},
  };

  for (size_t i = 0; i < sizeof(table) / sizeof(*table); ++i) {
    const char* string = table[i].string;
    assert(unicode_utf8_next(&string) == table[i].cp);
  }

  assert(unicode_properties_is_uppercase(unicode_cp_properties('A')));
  assert(unicode_properties_is_lowercase(unicode_cp_properties('A')));

  return EXIT_SUCCESS;
}
