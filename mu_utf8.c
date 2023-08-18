#include "mu_utf8.h"

char32_t mu_utf8_next(const char** utf8) {
  static const int TABLE[][3] = {
    {0x80, 0x00, 0x7F},
    {0xE0, 0xC0, 0x1F},
    {0xF0, 0xE0, 0x0F},
    {0xF8, 0xF0, 0x07},
  };

  for (size_t i = 0; i < sizeof(TABLE) / sizeof(*TABLE); ++i) {
    if (((*utf8)[0] & TABLE[i][0]) == TABLE[i][1]) {
      size_t length = i + 1;
      char32_t cp = (*utf8)[0] & TABLE[i][2];

      for (size_t i = 1; i < length; ++i) {
        if (((*utf8)[i] & 0xC0) != 0x80)
          return -1;

        cp = (cp << 6) | ((*utf8)[i] & 0x3F);
      }

      if (length != mu_cp_utf8_length(cp))
        return -1;

      utf8 += length;
      return cp;
    }
  }

  return -1;
}

size_t mu_cp_utf8_length(char32_t cp) {
  static const char32_t TABLE[] =
    {0xFFFFFF80, 0xFFFFF800, 0xFFFF0000, 0xFFE00000};

  for (size_t i = 0; i < sizeof(TABLE) / sizeof(*TABLE); ++i) {
    if ((cp & TABLE[i]) == 0)
      return i + 1;
  }

  return 0;
}
