#include "mu_properties.h"

#include <stddef.h>

struct MuProperties {
  const char* const name;
  MuCategory category;
  bool is_other_uppercase;
  bool is_other_lowercase;
};

typedef struct _PropertiesTable {
  const size_t count;
  const MuProperties* const head;
} _PropertiesTable;

#include "mu_properties.inc"

static const MuProperties _PROPERTIES_PRIVATE_USE =
  {NULL, MU_PRIVATE_USE, false, false};

static const MuProperties _PROPERTIES_UNASSIGNED =
  {NULL, MU_UNASSIGNED, false, false};

const MuProperties* mu_cp_properties(char32_t cp) {
  if (cp > 0x10FFFF)
    return NULL;

  const _PropertiesTable* table = _PROPERTIES_TABLES + (cp >> 16);

  if ((cp & 0xFFFF) < table->count) {
    const MuProperties* properties = table->head + (cp & 0xFFFF);

    if (properties != NULL)
      return properties;
  }

  if (cp >= 0xF0000 && cp < 0xFFFFE)
    return &_PROPERTIES_PRIVATE_USE;

  if (cp >= 0x100000 && cp < 0x10FFFE)
    return &_PROPERTIES_PRIVATE_USE;

  return &_PROPERTIES_UNASSIGNED;
}

const char* mu_properties_name(const MuProperties* properties) {
  return properties->name;
}

MuCategory mu_properties_category(const MuProperties* properties) {
  return properties->category;
}

bool mu_properties_is_uppercase(const MuProperties* properties) {
  return properties->category == MU_UPPERCASE_LETTER || properties->is_other_uppercase;
}

bool mu_properties_is_lowercase(const MuProperties* properties) {
  return properties->category == MU_LOWERCASE_LETTER || properties->is_other_lowercase;
}
