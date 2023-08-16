import re
import xml.etree.ElementTree as ET
from typing import Optional


GENERAL_CATEGORIES = {
    'Lu': 'UNICODE_UPPERCASE_LETTER',
    'Ll': 'UNICODE_LOWERCASE_LETTER',
    'Lt': 'UNICODE_TITLECASE_LETTER',
    'Lm': 'UNICODE_MODIFIER_LETTER',
    'Lo': 'UNICODE_OTHER_LETTER',
    'Mn': 'UNICODE_NONSPACING_MARK',
    'Mc': 'UNICODE_SPACING_MARK',
    'Me': 'UNICODE_ENCLOSING_MARK',
    'Nd': 'UNICODE_DECIMAL_NUMBER',
    'Nl': 'UNICODE_LETTER_NUMBER',
    'No': 'UNICODE_OTHER_NUMBER',
    'Pc': 'UNICODE_CONNECTOR_PUNCTUATION',
    'Pd': 'UNICODE_DASH_PUNCTUATION',
    'Ps': 'UNICODE_OPEN_PUNCTUATION',
    'Pe': 'UNICODE_CLOSE_PUNCTUATION',
    'Pi': 'UNICODE_INITIAL_PUNCTUATION',
    'Pf': 'UNICODE_FINAL_PUNCTUATION',
    'Po': 'UNICODE_OTHER_PUNCTUATION',
    'Sm': 'UNICODE_MATH_SYMBOL',
    'Sc': 'UNICODE_CURRENCY_SYMBOL',
    'Sk': 'UNICODE_MODIFIER_SYMBOL',
    'So': 'UNICODE_OTHER_SYMBOL',
    'Zs': 'UNICODE_SPACE_SEPARATOR',
    'Zl': 'UNICODE_LINE_SEPARATOR',
    'Zp': 'UNICODE_PARAGRAPH_SEPARATOR',
    'Cc': 'UNICODE_CONTROL',
    'Cf': 'UNICODE_FORMAT',
    'Cs': 'UNICODE_SURROGATE',
    'Co': 'UNICODE_PRIVATE_USE',
    'Cn': 'UNICODE_UNASSIGNED',
}


class Properties:
    def __init__(self) -> None:
        self.name = ''
        self.general_category = ''

    def __str__(self) -> str:
        name = f'"{self.name}"' if self.name != '' else 'NULL'
        general_catrgory = GENERAL_CATEGORIES[self.general_category] if self.general_category != '' else '-1'
        return f'{{{name}, {general_catrgory}}}'


def handle_code_point(
    code_point: ET.Element,
    properties_maps: list[list[Properties]],
    group: Optional[ET.Element] = None
) -> None:
    cp = ''
    first_cp = ''
    last_cp = ''
    name = ''
    general_category = ''

    for element in [code_point] if group is None else [group, code_point]:
        cp = element.get('cp', cp)
        first_cp = element.get('first-cp', first_cp)
        last_cp = element.get('last-cp', last_cp)
        name = element.get('na', name)
        general_category = element.get('gc', general_category)

    if cp != '':
        cp = int(cp, 16)
        first_cp = cp
        last_cp = cp
    else:
        first_cp = int(first_cp, 16)
        last_cp = int(last_cp, 16)

    for name_alias in code_point.iterfind('{*}name-alias[@type="correction"]'):
        name = name_alias.get('alias', name)

    for cp in range(first_cp, last_cp + 1):
        properties_maps[cp >> 16][cp & 0xFFFF].name = name.replace('#', f'{cp:04X}')
        properties_maps[cp >> 16][cp & 0xFFFF].general_category = general_category


if __name__ == '__main__':
    maps = [[Properties() for _ in range(0xFFFF + 1)] for _ in range(0x10 + 1)]
    tree = ET.parse('ucd.all.grouped.xml')

    for element in tree.iterfind('{*}repertoire/*'):
        if re.fullmatch(r'{.*}group', element.tag) is None:
            handle_code_point(element, maps)
        else:
            for code_point in element.iterfind('*'):
                handle_code_point(code_point, maps, element)

    with open('unicode_database.inc', 'w') as f:
        f.write('static const _Unicode_properties_map _unicode_properties_maps[] = {\n')

        for i, map in enumerate(maps):
            while True:  # Exclude last surrogate, private use, and not assigned code points
                updated = False

                for i in range(len(map) - 1, -1, -1):
                    if map[i].general_category in ['Cs', 'Co', 'Cn']:
                        del map[i]
                        updated = True
                    else:
                        break

                if not updated:
                    break

            if map == []:
                f.write('  {0, NULL},\n')
            else:
                f.write(f'  {{{len(map)}, (const _Unicode_properties[]){{\n')

                for properties in map:
                    f.write(f'    {properties},\n')

                f.write('  }},\n')

        f.write('};\n')
