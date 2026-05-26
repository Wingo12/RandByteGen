# RandByteGen
Random bytes generation
Usage: ./randbytegen [OPTIONS]
Options:
  -b, --bytes         : HEX формат (FF 48 00 ...)
  -p, --printable     : только печатные ASCII символы
  -d <int>            : десятичные числа, <int> штук в строке
  -c, --color <color> : цвет вывода
                       <color> = HEX (#RRGGBB или RRGGBB)
                               = слово (RED, GREEN, BLUE, ...)
                               = RANDOM (случайный цвет каждый раз)
  -i, --interval <ms> : интервал между выводами в миллисекундах
                       (по умолчанию: 200 мс = 5 раз в секунду)
  -h, --help          : показать эту справку

Examples:
  ./randbytegen -b -c RED                    # HEX байты красным цветом
  ./randbytegen -p -c RANDOM -i 500          # Печатные символы, 2/сек, случайные цвета
  ./randbytegen -d 3 -c #FF69B4 -i 100       # 3 числа, 10/сек, розовым
