# char8

c++20 向けの文字コード変換実験。
`char8_t` とか `std::string_view` などの新しめの要素を使ってみる。

* `std::u8string_view` は `std::basic_string_view<char8_t>`

```
UNICODE <> UTF-8
^
table
v
JISX0213 <> EUC-JP
^      \
v       \
SJIS     (ISO-2022-JP)
```

* iso-2022-jp は エスケープシーケンスが面倒なので後回しに

## TODO

* unicode
    * [x] unicode: block list
    * [ ] unicode: grapheme cluster
    * [x] utf8: to unicode
    * [x] utf8: from unicode
    * [ ] utf8: codepoint iterator
* table
    * [x] JISX0213 <=> BMP: U+0000~U+FFFF
    * [ ] JISX0213 <=> SMP: U+10000~U+1FFFF
    * [ ] JISX0213 <=> SIP: U+20000~U+2FFFF
* [x] jis(JISX0213): to unicode
* [x] jis(JISX0213): from unicode
    * [x] sjis: to jis
    * [x] sjis: from jis
    * [ ] euc-jp: to jis
    * [ ] euc-jp: from jis
    * [ ] iso-2022-jp: to jis
    * [ ] iso-2022-jp: from jis

## data

### JISX0213

* http://x0213.org/codetable/jisx0213-2004-8bit-std.txt
* http://www.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/JIS/JIS0208.TXT

### 文字一覧

* https://www.unicode.org/Public/UCD/latest/ucd/NamesList.txt

### Unicode blocks

* https://www.unicode.org/Public/UNIDATA/Blocks.txt
