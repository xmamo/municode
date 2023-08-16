#include "unicode.h"

#include <stdbool.h>
#include <stddef.h>

struct Unicode_properties {
  const char* const name;
  Unicode_general_category general_category;
  bool is_other_uppercase;
  bool is_other_lowercase;
};

typedef struct _Unicode_properties_table {
  const size_t count;
  const Unicode_properties* const properties;
} _Unicode_properties_table;

#include "unicode_cp_properties.inc"

static const Unicode_properties _unicode_properties_private_use =
  {NULL, UNICODE_PRIVATE_USE, false, false};

static const Unicode_properties _unicode_properties_unassigned =
  {NULL, UNICODE_UNASSIGNED, false, false};

const Unicode_properties* unicode_cp_properties(char32_t cp) {
  if (cp > 0x10FFFF)
    return NULL;

  const _Unicode_properties_table* table = _unicode_properties_tables + (cp >> 16);

  if ((cp & 0xFFFF) < table->count) {
    const Unicode_properties* properties = table->properties + (cp & 0xFFFF);

    if (properties != NULL)
      return properties;
  }

  if (cp >= 0xF0000 && cp < 0xFFFFE)
    return &_unicode_properties_private_use;

  if (cp >= 0x100000 && cp < 0x10FFFE)
    return &_unicode_properties_private_use;

  return &_unicode_properties_unassigned;
}

const char* unicode_properties_name(const Unicode_properties* properties) {
  return properties->name;
}

Unicode_general_category unicode_properties_general_category(const Unicode_properties* properties) {
  return properties->general_category;
}

bool unicode_properties_is_uppercase(const Unicode_properties* properties) {
  return properties->general_category == UNICODE_UPPERCASE_LETTER || properties->is_other_uppercase;
}

bool unicode_properties_is_lowercase(const Unicode_properties* properties) {
  return properties->general_category == UNICODE_LOWERCASE_LETTER || properties->is_other_lowercase;
}

bool unicode_properties_is_other_uppercase(const Unicode_properties* properties) {
  return properties->is_other_uppercase;
}

bool unicode_properties_is_other_lowercase(const Unicode_properties* properties) {
  return properties->is_other_lowercase;
}
