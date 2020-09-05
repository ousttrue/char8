import pathlib
from typing import List, Tuple

HERE = pathlib.Path(__file__).absolute().parent
BMP = 65536

# def endian(sjis):
#     if sjis == '0':
#         return sjis
#     return f'0x{sjis[4:6]}{sjis[2:4]}'


def map(src: pathlib.Path, dst: pathlib.Path):
    from_unicode = {}
    max_unicode = 0
    to_unicode = {}
    max_jis = 0
    for l in src.read_text().splitlines():
        if l.startswith('#'):
            continue
        splited = l.split('\t', maxsplit=2)
        jis, unicode, name = splited
        try:
            # may fail
            #
            # ex:
            # 0x2477	U+304B+309A	# 	[2000]
            unicode = int(unicode[2:], 16)

            if unicode>max_unicode and unicode < BMP:
                max_unicode = unicode
            jis = int(jis, 16)
            if jis>max_jis:
                max_jis = jis

            from_unicode[unicode] = (jis, name[2:])
            to_unicode[jis] = (unicode, name[2:])
        except:
            pass      

    with dst.open('w') as f:
        f.write('''///
/// generated from
/// http://x0213.org/codetable/jisx0213-2004-8bit-std.txt
///
#pragma once
''')

        # from unicode
        f.write('// only BMP(Basic Multilingual Plane)\n')
        f.write(
            'const unsigned short unicode_to_jisx0213[] = {\n'
        )
        for i in range(max_unicode+1):
            try:
                v = from_unicode[i]
                jis, name = v
                f.write(
                    f'    {hex(jis)}, // {name}\n'
                )
            except KeyError:
                f.write('    0,\n')
        f.write('};\n')

        # to unicode
        f.write('// start with 0x2121 \n')
        f.write('const unsigned int jisx0213_to_unicode[] = {\n')
        for i in range(max_jis+1):
            if i < 0x2121:
                continue
            try:
                v = to_unicode[i]
                unicode, name = v
                f.write(f'    {hex(unicode)}, // {name}\n')
            except KeyError:
                f.write(f'    0,\n')
        f.write('};\n')


if __name__ == '__main__':
    map(HERE / 'jisx0213-2004-8bit-std.txt', HERE.parent / 'include/char8/unicode_jisx0213.map.hpp')
