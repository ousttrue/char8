import pathlib
from typing import List, Tuple

HERE = pathlib.Path(__file__).absolute().parent


def endian(sjis):
    if sjis == '0':
        return sjis
    return f'0x{sjis[4:6]}{sjis[2:4]}'


def map(src: pathlib.Path, dst: pathlib.Path):
    from_unicode = [('0', '0', '') for x in range(65536)]
    to_unicode = [('0', '0', '') for x in range(65536)]
    for l in src.read_text().splitlines():
        if l.startswith('#'):
            continue
        # 0x8140	0x2121	0x3000	# IDEOGRAPHIC SPACE
        sjis, jis0208, unicode, name = l.split('\t', maxsplit=4)
        from_unicode[int(unicode, 16)] = (sjis, jis0208, name[2:])
        to_unicode[int(endian(jis0208), 16)] = (unicode, jis0208, name[2:])

    with dst.open('w') as f:
        f.write('''///
/// http://www.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/JIS/JIS0208.TXT
///
#pragma once
#include <stdint.h>
#include <array>

namespace c8::jisx0208 {
''')

        # unicode_to_sjis
        f.write(
            '   const std::array<uint8_t, 2> unicode_to_jisx0208[65536] = {\n'
        )
        for i, v in enumerate(from_unicode):
            sjis, jis0208, name = v
            if name:
                f.write(
                    f'        {{0x{jis0208[2:4]}, 0x{jis0208[4:6]}}}, // {name}\n'
                )
            else:
                f.write('        {0, 0},\n')
        f.write('    };\n')

        # sjis_to_unicode
        f.write('    const uint32_t jisx0208_to_unicode[65536] = {\n')
        for i, v in enumerate(to_unicode):
            unicode, jis0208, name = v
            if name:
                f.write(f'        {unicode}, // {name}\n')
            else:
                f.write(f'        0,\n')
        f.write('    };\n')

        f.write('}\n')


if __name__ == '__main__':
    map(HERE / 'JIS0208.TXT', HERE.parent / 'include/char8/jis0208.map.hpp')
