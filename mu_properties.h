#ifndef MU_PROPERTIES_H
#define MU_PROPERTIES_H

#include <stdbool.h>
#include <stdint.h>

typedef uint_least32_t char32_t;

typedef enum MuCategory {
  MU_UPPERCASE_LETTER,
  MU_LOWERCASE_LETTER,
  MU_TITLECASE_LETTER,
  MU_MODIFIER_LETTER,
  MU_OTHER_LETTER,
  MU_NONSPACING_MARK,
  MU_SPACING_MARK,
  MU_ENCLOSING_MARK,
  MU_DECIMAL_NUMBER,
  MU_LETTER_NUMBER,
  MU_OTHER_NUMBER,
  MU_CONNECTOR_PUNCTUATION,
  MU_DASH_PUNCTUATION,
  MU_OPEN_PUNCTUATION,
  MU_CLOSE_PUNCTUATION,
  MU_INITIAL_PUNCTUATION,
  MU_FINAL_PUNCTUATION,
  MU_OTHER_PUNCTUATION,
  MU_MATH_SYMBOL,
  MU_CURRENCY_SYMBOL,
  MU_MODIFIER_SYMBOL,
  MU_OTHER_SYMBOL,
  MU_SPACE_SEPARATOR,
  MU_LINE_SEPARATOR,
  MU_PARAGRAPH_SEPARATOR,
  MU_CONTROL,
  MU_FORMAT,
  MU_SURROGATE,
  MU_PRIVATE_USE,
  MU_UNASSIGNED,
} MuCategory;

typedef struct MuProperties MuProperties;

const MuProperties* mu_cp_properties(char32_t cp);

const char* mu_properties_name(const MuProperties* properties);

MuCategory mu_properties_category(const MuProperties* properties);

bool mu_properties_is_uppercase(const MuProperties* properties);

bool mu_properties_is_lowercase(const MuProperties* properties);

#endif
