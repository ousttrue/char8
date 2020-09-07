import pathlib
import re

HERE = pathlib.Path(__file__).absolute().parent

PATTERN = re.compile(r'^(\w+)\.\.(\w+); (.*)')

def to_enum_name(name: str)->str:
    return name.upper().replace(' ', '_').replace('-', '_')

def process(path: pathlib.Path, dst: pathlib.Path):
    # print(path)

    with dst.open('w') as f:
        f.write('''///
/// generated from
/// https://www.unicode.org/Public/UNIDATA/Blocks.txt
///
''')

        data = path.read_text()
        lines = data.splitlines()
        blocks = []
        for l in lines:
            l = l.strip()
            if not l:
                continue
            if l.startswith('#'):
                continue

            m = PATTERN.match(l)
            begin = int(m.group(1), 16)
            end = int(m.group(2), 16)
            name = m.group(3)
            blocks.append((name, begin, end))

        #
        # enum
        #
        f.write('''
enum class UnicodeBlocks {
    UNKNOWN = -1,
''')
        for b in blocks:
            name, begin, end = b
            f.write(f'    {to_enum_name(name)} = {hex(begin)},\n')
        f.write('''
};
''')

        #
        # blocks
        #
        f.write('''
struct Range
{
    std::string name;
    uint32_t front;
    uint32_t back;
};
static inline Range blocks[] = {
''')
        for b in blocks:
            name, begin, end = b
            # print(f'{name}: {begin} to {end}')
            f.write(f'  {{"{name}", {hex(begin)}, {hex(end)}}},\n')
            # print(l)
        f.write('''
};
''')

if __name__=='__main__':
    process(HERE / 'Blocks.txt', HERE.parent / 'include/char8/unicode.blocks.hpp')
