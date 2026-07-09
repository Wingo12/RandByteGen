
# RandByteGen

[![Version](https://img.shields.io/badge/version-v0.4-blue)](https://github.com/Wingo12/RandByteGen/releases/tag/v0.4)
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)

**RandByteGen** — лёгкий генератор случайных байтов с поддержкой цветного вывода, градиентов, файлового экспорта и мультиязычности.

---

## ✨ Features / Возможности

- **3 режима / 3 modes**: HEX, печатные символы, десятичные числа
- **Цвета / Colors**: имена, HEX-коды, RANDOM, ГРАДИЕНТЫ (color1:color2)
- **Смена цвета / Color switching**: по секундам, строкам, символам
- **Файловый вывод / File output**: с интерактивным выбором (ANSI или plain text)
- **Мультиязычность / Multi-language**: русский / английский (авто + выбор)
- **Кроссплатформенность / Cross-platform**: Linux, macOS, Windows
- **Системная информация / System info**: --sysinfo

---

## 📦 Installation / Установка

```bash
git clone https://github.com/Wingo12/RandByteGen.git
cd RandByteGen
make
sudo make install
```

---

🚀 Usage / Использование

```
./randbytegen [OPTIONS] / [ОПЦИИ]
```

🇬🇧 English

| Option | Description |
|--------|-------------|
| `-b, --bytes` | HEX format (FF 48 00 ...) |
| `-p, --printable` | Printable ASCII characters only |
| `-d, --decimals` | Decimal numbers |
| `-l, --line <int>` | Elements per line |
| `-a, --amount <int>` | Total elements to output (0 = infinite) |
| `-c, --color <color>` | Output color: name, HEX, RANDOM, or gradient |
| `-s, --switch <mode>:<value>` | Color switching: `sec`, `line`, `char` |
| `-i, --interval <ms>` | Interval between outputs (default: 200 ms) |
| `-o, --output <file>` | Save output to file |
| `--sysinfo` | Show system information |
| `-v, --version` | Show version and author information |
| `-h, --help` | Show this help |

🇷🇺 Русский

| Опция | Описание |
|-------|----------|
| `-b, --bytes` | HEX формат (FF 48 00 ...) |
| `-p, --printable` | Только печатные ASCII символы |
| `-d, --decimals` | Десятичные числа |
| `-l, --line <int>` | Количество элементов в строке |
| `-a, --amount <int>` | Количество элементов для вывода (0 = бесконечно) |
| `-c, --color <color>` | Цвет вывода: имя, HEX, RANDOM или градиент |
| `-s, --switch <режим>:<значение>` | Смена цвета: `sec`, `line`, `char` |
| `-i, --interval <мс>` | Интервал между выводами (по умолч. 200 мс) |
| `-o, --output <файл>` | Сохранить вывод в файл |
| `--sysinfo` | Показать информацию о системе |
| `-v, --version` | Показать версию и информацию об авторе |
| `-h, --help` | Показать эту справку |

---

📝 Examples / Примеры

🇬🇧 English

```bash
# HEX bytes, 5 per line, 20 total, red color
./randbytegen -b -l 5 -a 20 -c RED

# Gradient from blue to red
./randbytegen -b -l 5 -a 20 -c blue:red

# Printable chars, random colors, switch every 2 lines
./randbytegen -p -l 3 -a 12 -c RANDOM -s line:2

# Save to file with confirmation
./randbytegen -d -l 4 -a 16 -c #FF00FF -o output.txt

# Show system info
./randbytegen --sysinfo

# Show version
./randbytegen -v
```

🇷🇺 Русский

```bash
# HEX байты, по 5 в строке, всего 20, красный цвет
./randbytegen -b -l 5 -a 20 -c RED

# Градиент от синего к красному
./randbytegen -b -l 5 -a 20 -c blue:red

# Печатные символы, случайные цвета, смена каждые 2 строки
./randbytegen -p -l 3 -a 12 -c RANDOM -s line:2

# Сохранение в файл с подтверждением
./randbytegen -d -l 4 -a 16 -c #FF00FF -o output.txt

# Показать информацию о системе
./randbytegen --sysinfo

# Показать версию
./randbytegen -v
```

---

📁 Version History / История версий

Version Description / Описание
v0.4 Gradient, switch, sysinfo, version flag, author, GitHub, multi-language, cross-platform
v0.1 Initial release: HEX, printable, decimal modes

---

👤 Author / Автор

Wingo123
GitHub: Wingo12

---

📄 License / Лицензия

MIT License — see LICENSE file.
Лицензия MIT — см. файл LICENSE.

---

🖥️ Platforms / Платформы

· ✅ Linux
· ✅ macOS
· ✅ Windows
