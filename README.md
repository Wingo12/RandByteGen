
# RandByteGen

**RandByteGen** — это легковесная утилита на C, которая генерирует случайные байты из `/dev/urandom` и выводит их в различных форматах с поддержкой цветного вывода и регулируемой скоростью.

---

## 🇬🇧 English

### Features
- Generate random bytes in **HEX**, **decimal**, or **printable ASCII** formats
- **Colored output** (named colors, HEX codes, or random colors)
- **Adjustable speed** (set custom interval in milliseconds)
- **High performance** — minimal CPU usage

### Installation

```bash
git clone https://github.com/Wingo12/RandByteGen.git
cd RandByteGen
make
```

Usage

```
./randbytegen [OPTIONS]
```

Option Description
- -b, --bytes HEX format (FF 48 00 ...)
- -p, --printable Printable ASCII characters only
- -d <int> Decimal numbers, <int> per line
- -c, --color <color> Output color: HEX (#RRGGBB), name (RED, GREEN, etc.), or RANDOM
- -i, --interval <ms> Interval in milliseconds (default: 200 ms = 5/sec)
- -h, --help Show help

Examples

```bash
./randbytegen -b -c RED                    # HEX bytes in red
./randbytegen -p -c RANDOM -i 500          # Printable chars, 2/sec, random colors
./randbytegen -d 3 -c #FF69B4 -i 100       # 3 decimals per line, 10/sec, pink
```

Available Color Names

RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, ORANGE, PURPLE, PINK, WHITE

License

MIT License — see LICENSE file.

---

🇷🇺 Русский

Возможности

· Генерация случайных байтов в HEX, десятичном или печатном ASCII форматах
· Цветной вывод (имена цветов, HEX-коды или случайные цвета)
· Регулируемая скорость (задание интервала в миллисекундах)
· Высокая производительность — минимальное потребление CPU

Установка

```bash
git clone https://github.com/Wingo12/RandByteGen.git
cd RandByteGen
make
```

Использование

```
./randbytegen [ОПЦИИ]
```

Опция Описание
- -b, --bytes HEX формат (FF 48 00 ...)
- -p, --printable Только печатные ASCII символы
- -d <int> Десятичные числа, <int> штук в строке
- -c, --color <color> Цвет вывода: HEX (#RRGGBB), имя (RED, GREEN...) или RANDOM
- -i, --interval <ms> Интервал в миллисекундах (по умолч. 200 мс = 5/сек)
- -h, --help Показать справку

Примеры

```bash
./randbytegen -b -c RED                    # HEX байты красным цветом
./randbytegen -p -c RANDOM -i 500          # Печатные символы, 2/сек, случайные цвета
./randbytegen -d 3 -c #FF69B4 -i 100       # 3 числа, 10/сек, розовым
```

Доступные имена цветов

RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, ORANGE, PURPLE, PINK, WHITE

Лицензия

Лицензия MIT — см. файл LICENSE.

---

📄 License / Лицензия

```
MIT License

Copyright (c) 2025

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

Building / Сборка

```bash
make          # build randbytegen
make clean    # remove object files and binary
make run      # test run (HEX + random colors)
```

Requirements / Требования

· Linux / macOS / any Unix-like system with /dev/urandom
· GCC or any C99-compatible compiler
- ***Спасибо за использование***
