#include "random_bytes.h"

void print_usage(const char *progname) {
    printf("Usage: %s [OPTIONS]\n", progname);
    printf("Options:\n");
    printf("  -b, --bytes         : HEX формат (FF 48 00 ...)\n");
    printf("  -p, --printable     : только печатные ASCII символы\n");
    printf("  -d <int>            : десятичные числа, <int> штук в строке\n");
    printf("  -c, --color <color> : цвет вывода\n");
    printf("                       <color> = HEX (#RRGGBB или RRGGBB)\n");
    printf("                               = слово (RED, GREEN, BLUE, ...)\n");
    printf("                               = RANDOM (случайный цвет каждый раз)\n");
    printf("  -i, --interval <ms> : интервал между выводами в миллисекундах\n");
    printf("                       (по умолчанию: 200 мс = 5 раз в секунду)\n");
    printf("  -h, --help          : показать эту справку\n");
    printf("\nExamples:\n");
    printf("  %s -b -c RED                    # HEX байты красным цветом\n", progname);
    printf("  %s -p -c RANDOM -i 500          # Печатные символы, 2/сек, случайные цвета\n", progname);
    printf("  %s -d 3 -c #FF69B4 -i 100       # 3 числа, 10/сек, розовым\n", progname);
}

Config parse_arguments(int argc, char *argv[]) {
    Config config = {
        .mode = 0,
        .count_per_line = 1,
        .color_mode = 0,
        .interval_ms = 200,
        .fixed_color = {255, 255, 255}
    };
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0) {
            config.mode = 0;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--printable") == 0) {
            config.mode = 1;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decimals") == 0) {
            config.mode = 2;
            if (i + 1 < argc) {
                config.count_per_line = atoi(argv[++i]);
                if (config.count_per_line < 1) config.count_per_line = 1;
            }
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--color") == 0) {
            if (i + 1 < argc) {
                i++;
                if (strcasecmp(argv[i], "RANDOM") == 0) {
                    config.color_mode = 2;
                } else if (parse_hex_color(argv[i], &config.fixed_color)) {
                    config.color_mode = 1;
                } else if (parse_color_name(argv[i], &config.fixed_color)) {
                    config.color_mode = 1;
                } else {
                    fprintf(stderr, "Error: Invalid color: %s\n", argv[i]);
                    exit(1);
                }
            }
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interval") == 0) {
            if (i + 1 < argc) {
                config.interval_ms = atoi(argv[++i]);
                if (config.interval_ms < 1) config.interval_ms = 1;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            exit(0);
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            exit(1);
        }
    }
    
    return config;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    Config config = parse_arguments(argc, argv);
    
    switch (config.mode) {
        case 0:
            print_bytes_mode(&config);
            break;
        case 1:
            print_printable_mode(&config);
            break;
        case 2:
            print_decimals_mode(&config);
            break;
    }
    
    return 0;
}
