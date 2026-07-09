#include "randbytegen.h"

void detect_os(Config *config) {
    #ifdef _WIN32
        config->os_type = 3;
        strcpy(config->os_name, "Windows");
    #elif __APPLE__
        config->os_type = 2;
        strcpy(config->os_name, "macOS");
    #elif __linux__
        config->os_type = 1;
        struct utsname u;
        if (uname(&u) == 0) {
            snprintf(config->os_name, sizeof(config->os_name), "Linux (%s %s)", u.sysname, u.release);
        } else {
            strcpy(config->os_name, "Linux");
        }
    #else
        config->os_type = 0;
        strcpy(config->os_name, "Unknown");
    #endif
}

int check_urandom() {
    #if defined(HAS_URANDOM)
        FILE *f = fopen("/dev/urandom", "r");
        if (f) { fclose(f); return 1; }
    #endif
    #ifdef _WIN32
        HCRYPTPROV hProv;
        if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
            CryptReleaseContext(hProv, 0);
            return 1;
        }
    #endif
    return 0;
}

unsigned char get_random_byte() {
    static unsigned int seed = 0;
    if (seed == 0) seed = time(NULL) ^ (getpid() << 16);
    #ifdef _WIN32
        unsigned int v;
        if (rand_s(&v) == 0) return (unsigned char)(v & 0xFF);
    #endif
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return (unsigned char)(seed & 0xFF);
}

void print_system_info(const Config *config) {
    printf(config->lang ? "📋 Информация о системе:\n" : "📋 System Information:\n");
    printf("   %s: %s\n", config->lang ? "ОС" : "OS", config->os_name);
    printf("   /dev/urandom: %s\n", check_urandom() ? (config->lang ? "✅ Доступен" : "✅ Available") : (config->lang ? "❌ Недоступен" : "❌ Not available"));
    printf("   %s: %s\n", config->lang ? "Цветной вывод" : "Color output", isatty(STDOUT_FILENO) ? (config->lang ? "✅ Поддерживается" : "✅ Supported") : (config->lang ? "❌ Не поддерживается" : "❌ Not supported"));
    printf("\n");
}

int detect_language() {
    char *env = getenv("RANDBYTEGEN_LANG");
    if (env) {
        if (strcasecmp(env, "ru") == 0 || strcasecmp(env, "russian") == 0) return 1;
        if (strcasecmp(env, "en") == 0 || strcasecmp(env, "english") == 0) return 0;
    }

    char *home = getenv("HOME");
    char rc_path[512];
    if (home) {
        snprintf(rc_path, sizeof(rc_path), "%s/.randbytegenrc", home);
    } else {
        snprintf(rc_path, sizeof(rc_path), "/tmp/.randbytegenrc");
    }

    FILE *rc = fopen(rc_path, "r");
    if (rc) {
        char lang[4];
        if (fgets(lang, sizeof(lang), rc)) {
            fclose(rc);
            if (strcmp(lang, "ru\n") == 0 || strcmp(lang, "ru") == 0) return 1;
            if (strcmp(lang, "en\n") == 0 || strcmp(lang, "en") == 0) return 0;
        }
        fclose(rc);
    }

    setlocale(LC_ALL, "");
    char *lang = getenv("LANG");
    if (lang && strncmp(lang, "ru", 2) == 0) return 1;
    #ifdef _WIN32
        LANGID lid = GetUserDefaultUILanguage();
        if (lid == 0x0419 || lid == 0x0819) return 1;
    #endif

    char choice[10];
    printf("\n🌐 Language selection / Выбор языка:\n");
    printf("   [1] English\n");
    printf("   [2] Русский\n");
    printf("   Your choice (1/2): ");
    while (1) {
        if (fgets(choice, sizeof(choice), stdin) == NULL) return 0;
        if (choice[0] == '1') {
            printf("   ✓ Language set to English\n");
            setenv("RANDBYTEGEN_LANG", "en", 1);
            rc = fopen(rc_path, "w");
            if (rc) { fprintf(rc, "en\n"); fclose(rc); }
            return 0;
        }
        if (choice[0] == '2') {
            printf("   ✓ Язык установлен на русский\n");
            setenv("RANDBYTEGEN_LANG", "ru", 1);
            rc = fopen(rc_path, "w");
            if (rc) { fprintf(rc, "ru\n"); fclose(rc); }
            return 1;
        }
        printf("   Invalid choice. Please enter 1 or 2: ");
    }
}

void print_version(const Config *config) {
    (void)config;
    printf("RandByteGen version " VERSION "\n");
    printf("Author: " AUTHOR "\n");
    printf("GitHub: " GITHUB "\n");
    printf("Platforms: Linux, macOS, Windows\n");
}

const char* get_usage_text(const Config *config) {
    if (config->lang) {
        return
            "Использование: %s [ОПЦИИ]\n"
            "Опции:\n"
            "  -b, --bytes         : HEX формат (FF 48 00 ...)\n"
            "  -p, --printable     : только печатные ASCII символы\n"
            "  -d, --decimals      : десятичные числа\n"
            "  -l, --line <int>    : количество элементов в строке\n"
            "  -a, --amount <int>  : количество элементов для вывода (0 = бесконечно)\n"
            "  -c, --color <color> : цвет вывода\n"
            "                       <color> = имя (RED, GREEN, ...)\n"
            "                               = HEX (#RRGGBB или RRGGBB)\n"
            "                               = RANDOM (случайный цвет)\n"
            "                               = градиент (color1:color2 или #hex1:#hex2)\n"
            "  -s, --switch <режим>:<значение> : смена цвета\n"
            "                       режимы: sec (секунды), line (строки), char (символы)\n"
            "  -i, --interval <ms> : интервал между выводами (по умолч. 200 мс)\n"
            "  -o, --output <file> : сохранить вывод в файл\n"
            "  --sysinfo           : показать информацию о системе\n"
            "  -v, --version       : показать версию и информацию об авторе\n"
            "  -h, --help          : показать эту справку\n"
            "\nПримеры:\n"
            "  %s -b -l 5 -a 20 -c RED                         # Красный цвет\n"
            "  %s -b -l 5 -a 20 -c blue:red                    # Градиент от синего к красному\n"
            "  %s -b -l 5 -a 20 -c #0000FF:#FF0000             # Градиент HEX\n"
            "  %s -d -l 3 -a 12 -c RANDOM -s line:2 -o out.txt # Сохранение в файл\n";
    } else {
        return
            "Usage: %s [OPTIONS]\n"
            "Options:\n"
            "  -b, --bytes         : HEX format (FF 48 00 ...)\n"
            "  -p, --printable     : printable ASCII characters only\n"
            "  -d, --decimals      : decimal numbers\n"
            "  -l, --line <int>    : elements per line\n"
            "  -a, --amount <int>  : total elements to output (0 = infinite)\n"
            "  -c, --color <color> : output color\n"
            "                       <color> = name (RED, GREEN, ...)\n"
            "                               = HEX (#RRGGBB or RRGGBB)\n"
            "                               = RANDOM (random color)\n"
            "                               = gradient (color1:color2 or #hex1:#hex2)\n"
            "  -s, --switch <mode>:<value> : color switching\n"
            "                       modes: sec (seconds), line (lines), char (characters)\n"
            "  -i, --interval <ms> : interval between outputs (default: 200 ms)\n"
            "  -o, --output <file> : save output to file\n"
            "  --sysinfo           : show system information\n"
            "  -v, --version       : show version and author information\n"
            "  -h, --help          : show this help\n"
            "\nExamples:\n"
            "  %s -b -l 5 -a 20 -c RED                         # Red color\n"
            "  %s -b -l 5 -a 20 -c blue:red                    # Gradient from blue to red\n"
            "  %s -b -l 5 -a 20 -c #0000FF:#FF0000             # HEX gradient\n"
            "  %s -d -l 3 -a 12 -c RANDOM -s line:2 -o out.txt # Save to file\n";
    }
}

void print_usage(const Config *config) {
    const char *fmt = get_usage_text(config);
    printf(fmt, "randbytegen", "randbytegen", "randbytegen", "randbytegen", "randbytegen");
}

const char* get_message(Config *config, int id) {
    if (config->lang) {
        switch(id) {
            case 0: return "Ошибка: Неверный формат switch. Используйте режим:значение (например, sec:2)";
            case 1: return "Ошибка: Значение switch должно быть >= 1";
            case 2: return "Ошибка: Неизвестный режим switch: %s (используйте sec, line или char)";
            case 3: return "Ошибка: Неверный цвет: %s";
            case 4: return "Ошибка: Неверный начальный цвет: %s";
            case 5: return "Ошибка: Неверный конечный цвет: %s";
            case 6: return "Ошибка: -o требует имя файла";
            case 17: return "⚠️ /dev/urandom не найден! Используем альтернативный генератор.";
            default: return "";
        }
    } else {
        switch(id) {
            case 0: return "Error: Invalid switch format. Use mode:value (e.g., sec:2)";
            case 1: return "Error: Switch value must be >= 1";
            case 2: return "Error: Unknown switch mode: %s (use sec, line, or char)";
            case 3: return "Error: Invalid color: %s";
            case 4: return "Error: Invalid start color: %s";
            case 5: return "Error: Invalid end color: %s";
            case 6: return "Error: -o requires a filename";
            case 17: return "⚠️ /dev/urandom not found! Using alternative generator.";
            default: return "";
        }
    }
}

Config parse_arguments(int argc, char *argv[]) {
    Config config = {0};
    config.line_count = 1;
    config.amount = 0;
    config.interval_ms = 200;
    config.color_mode = 0;
    config.gradient_steps = 10;
    config.fixed_color = (Color){255, 255, 255};
    config.color_start = (Color){255, 255, 255};
    config.color_end = (Color){255, 255, 255};
    config.lang = detect_language();
    detect_os(&config);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--sysinfo") == 0) {
            print_system_info(&config);
            exit(0);
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version(&config);
            exit(0);
        }
    }

    if (!check_urandom()) {
        fprintf(stderr, "%s\n", get_message(&config, 17));
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes") == 0) {
            config.mode = 0;
        } else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--printable") == 0) {
            config.mode = 1;
        } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--decimals") == 0) {
            config.mode = 2;
        } else if (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--line") == 0) {
            if (i + 1 < argc) {
                config.line_count = atoi(argv[++i]);
                if (config.line_count < 1) config.line_count = 1;
            }
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--amount") == 0) {
            if (i + 1 < argc) {
                config.amount = atoi(argv[++i]);
                if (config.amount < 0) config.amount = 0;
            }
        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--color") == 0) {
            if (i + 1 < argc) {
                i++;
                char *arg = argv[i];
                char *colon = strchr(arg, ':');
                if (colon) {
                    *colon = '\0';
                    char *c1 = arg;
                    char *c2 = colon + 1;
                    if (!parse_color_name(c1, &config.color_start) && !parse_hex_color(c1, &config.color_start)) {
                        fprintf(stderr, get_message(&config, 4), c1);
                        exit(1);
                    }
                    if (!parse_color_name(c2, &config.color_end) && !parse_hex_color(c2, &config.color_end)) {
                        fprintf(stderr, get_message(&config, 5), c2);
                        exit(1);
                    }
                    config.color_mode = 3;
                } else if (strcasecmp(arg, "RANDOM") == 0) {
                    config.color_mode = 2;
                } else if (parse_hex_color(arg, &config.fixed_color)) {
                    config.color_mode = 1;
                } else if (parse_color_name(arg, &config.fixed_color)) {
                    config.color_mode = 1;
                } else {
                    fprintf(stderr, get_message(&config, 3), arg);
                    exit(1);
                }
            }
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--switch") == 0) {
            if (i + 1 < argc) {
                char *arg = argv[++i];
                char *colon = strchr(arg, ':');
                if (!colon) {
                    fprintf(stderr, "%s\n", get_message(&config, 0));
                    exit(1);
                }
                *colon = '\0';
                char *mode = arg;
                int value = atoi(colon + 1);
                if (value < 1) {
                    fprintf(stderr, "%s\n", get_message(&config, 1));
                    exit(1);
                }
                if (strcmp(mode, "sec") == 0) {
                    config.switch_mode = 1;
                    config.switch_value = value;
                } else if (strcmp(mode, "line") == 0) {
                    config.switch_mode = 2;
                    config.switch_value = value;
                } else if (strcmp(mode, "char") == 0) {
                    config.switch_mode = 3;
                    config.switch_value = value;
                } else {
                    fprintf(stderr, get_message(&config, 2), mode);
                    exit(1);
                }
            }
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interval") == 0) {
            if (i + 1 < argc) {
                config.interval_ms = atoi(argv[++i]);
                if (config.interval_ms < 1) config.interval_ms = 1;
            }
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (i + 1 < argc) {
                config.output_file = argv[++i];
            } else {
                fprintf(stderr, "%s\n", get_message(&config, 6));
                exit(1);
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(&config);
            printf("\n");
            print_system_info(&config);
            exit(0);
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage(&config);
            exit(1);
        }
    }
    return config;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    Config config = parse_arguments(argc, argv);

    switch (config.mode) {
        case 0: print_bytes_mode(&config); break;
        case 1: print_printable_mode(&config); break;
        case 2: print_decimals_mode(&config); break;
    }
    return 0;
}