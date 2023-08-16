#include "unicode.h"

#include <assert.h>
#include <stdbool.h>

typedef struct _Unicode_properties {
  const char* const name;
  Unicode_general_category general_category;
} _Unicode_properties;

typedef struct _Unicode_properties_map {
  const size_t count;
  const _Unicode_properties* const properties;
} _Unicode_properties_map;

#include "unicode_database.inc"

static bool _unicode_properties(char32_t cp, const _Unicode_properties** out_properties) {
  if (cp > 0x10FFFF)
    return false;

  const _Unicode_properties_map* map = _unicode_properties_maps + (cp >> 16);
  *out_properties = (cp & 0xFFFF) < map->count ? map->properties + (cp & 0xFFFF) : NULL;
  return true;
}

const char* unicode_name(char32_t cp) {
  const _Unicode_properties* properties = NULL;
  _unicode_properties(cp, &properties);
  return properties != NULL ? properties->name : NULL;
}

Unicode_general_category unicode_general_category(char32_t cp) {
  const _Unicode_properties* properties;

  if (!_unicode_properties(cp, &properties))
    return -1;

  if (properties != NULL)
    return properties->general_category;

#if false
  if (cp >= 0xD800 && cp <= 0xDFFF)
    return UNICODE_SURROGATE;

  if (cp >= 0xE000 && cp <= 0xF8FF)
    return UNICODE_PRIVATE_USE;
#endif

  if (cp >= 0xF0000 && cp < 0xFFFFE)
    return UNICODE_PRIVATE_USE;

  if (cp >= 0x100000 && cp < 0x10FFFE)
    return UNICODE_PRIVATE_USE;

  return UNICODE_UNASSIGNED;
}
