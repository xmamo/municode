import re
import xml.etree.ElementTree as ET
from typing import Optional


class Properties:
    def __init__(self) -> None:
        self.name = None
        self.general_category = None
        self.other_uppercase = False
        self.other_lowercase = False

    def set_name(self, name: str) -> None:
        self.name = name if name != '' else None

    def set_general_category(self, general_category: str) -> None:
        self.general_category = {
            'Lu': 'UPPERCASE_LETTER',
            'Ll': 'LOWERCASE_LETTER',
            'Lt': 'TITLECASE_LETTER',
            'Lm': 'MODIFIER_LETTER',
            'Lo': 'OTHER_LETTER',
            'Mn': 'NONSPACING_MARK',
            'Mc': 'SPACING_MARK',
            'Me': 'ENCLOSING_MARK',
            'Nd': 'DECIMAL_NUMBER',
            'Nl': 'LETTER_NUMBER',
            'No': 'OTHER_NUMBER',
            'Pc': 'CONNECTOR_PUNCTUATION',
            'Pd': 'DASH_PUNCTUATION',
            'Ps': 'OPEN_PUNCTUATION',
            'Pe': 'CLOSE_PUNCTUATION',
            'Pi': 'INITIAL_PUNCTUATION',
            'Pf': 'FINAL_PUNCTUATION',
            'Po': 'OTHER_PUNCTUATION',
            'Sm': 'MATH_SYMBOL',
            'Sc': 'CURRENCY_SYMBOL',
            'Sk': 'MODIFIER_SYMBOL',
            'So': 'OTHER_SYMBOL',
            'Zs': 'SPACE_SEPARATOR',
            'Zl': 'LINE_SEPARATOR',
            'Zp': 'PARAGRAPH_SEPARATOR',
            'Cc': 'CONTROL',
            'Cf': 'FORMAT',
            'Cs': 'SURROGATE',
            'Co': 'PRIVATE_USE',
            'Cn': 'UNASSIGNED',
            '': None,
        }[general_category]

    def set_other_uppercase(self, other_uppercase: str) -> None:
        self.other_uppercase = {'N': False, 'Y': True}[other_uppercase]

    def set_other_lowercase(self, other_lowercase: str) -> None:
        self.other_lowercase = {'N': False, 'Y': True}[other_lowercase]

    def __str__(self) -> str:
        name = 'NULL' if self.name is None else f'"{self.name}"'
        general_catrgory = '-1' if self.general_category is None else f'MU_{self.general_category}'
        other_uppercase = str(self.other_uppercase).lower()
        other_lowercase = str(self.other_lowercase).lower()
        return f'{{{name}, {general_catrgory}, {other_uppercase}, {other_lowercase}}}'


def handle_code_point(
    code_point: ET.Element,
    properties_tables: list[list[Properties]],
    group: Optional[ET.Element] = None
) -> None:
    cp = ''
    first_cp = ''
    last_cp = ''
    name = ''
    general_category = ''
    other_uppercase = ''
    other_lowercase = ''

    for element in [code_point] if group is None else [group, code_point]:
        cp = element.get('cp', cp)
        first_cp = element.get('first-cp', first_cp)
        last_cp = element.get('last-cp', last_cp)
        name = element.get('na', name)
        general_category = element.get('gc', general_category)
        other_uppercase = element.get('OUpper', other_uppercase)
        other_lowercase = element.get('OLower', other_lowercase)

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
        properties = properties_tables[cp >> 16][cp & 0xFFFF]
        properties.set_name(name.replace('#', f'{cp:04X}'))
        properties.set_general_category(general_category)
        properties.set_other_uppercase(other_uppercase)
        properties.set_other_lowercase(other_lowercase)


if __name__ == '__main__':
    tables = [[Properties() for _ in range(0xFFFF + 1)] for _ in range(0x10 + 1)]
    tree = ET.parse('ucd.all.grouped.xml')

    for element in tree.iterfind('{*}repertoire/*'):
        if re.fullmatch(r'{.*}group', element.tag) is None:
            handle_code_point(element, tables)
        else:
            for code_point in element.iterfind('*'):
                handle_code_point(code_point, tables, element)

    with open('unicode_cp_properties.inc', 'w') as f:
        f.write('static const _PropertiesTable _PROPERTIES_TABLES[] = {\n')

        for i, table in enumerate(tables):
            for j in range(len(table) - 1, -1, -1):
                if table[j].general_category in ['SURROGATE', 'PRIVATE_USE', 'UNASSIGNED']:
                    del table[j]
                else:
                    break

            if table == []:
                f.write('  {0, NULL},\n')
            else:
                f.write(f'  {{{len(table)}, (const MuProperties[]){{\n')

                for cp, properties in enumerate(table):
                    f.write(f'    /* U+{((i << 16) | cp):04X} */ {properties},\n')

                f.write('  }},\n')

        f.write('};\n')
