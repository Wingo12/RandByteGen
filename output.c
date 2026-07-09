#include "randbytegen.h"

extern unsigned char get_random_byte();

static void sleep_ms(int ms) {
    struct timespec req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&req, NULL);
}

static int read_byte(unsigned char *byte, Config *config) {
    (void)config;
    #if defined(HAS_URANDOM)
        static FILE *urandom = NULL;
        if (!urandom) {
            urandom = fopen("/dev/urandom", "r");
        }
        if (urandom && fread(byte, 1, 1, urandom) == 1) {
            return 1;
        }
    #endif
    *byte = get_random_byte();
    return 1;
}

static void write_output(const char *line, Config *config, Color color) {
    if (config->output_file) {
        if (config->preserve_colors) {
            fprintf(config->output_fp, "%s%s\033[0m\n", color_to_ansi(color), line);
        } else {
            fprintf(config->output_fp, "%s\n", line);
        }
        fflush(config->output_fp);
    } else {
        print_colored(line, color);
    }
}

int confirm_color_save(Config *config) {
    if (!config->output_file || config->color_mode == 0) return 1;
    
    char choice;
    printf("\n%s\n", config->lang ? "⚠️ Предупреждение: Обнаружен цветной вывод!" : "⚠️ Warning: Color output detected!");
    printf(config->lang ? "   Вы сохраняете в файл '%s' с цветовыми кодами.\n" : "   You are saving to file '%s' with color codes.\n", config->output_file);
    printf("%s\n", config->lang ? "   Цвета будут сохранены как ANSI escape-коды." : "   Colors will be saved as ANSI terminal escape codes.");
    printf("%s\n", config->lang ? "   [y] Сохранить с кодами   [n] Сохранить без кодов   [a] Отмена" : "   [y] Save with codes   [n] Save without codes   [a] Abort");
    printf("%s ", config->lang ? "   Ваш выбор (y/n/a): " : "   Your choice (y/n/a): ");
    
    while (1) {
        choice = getchar();
        if (choice == '\n') continue;
        
        if (choice == 'y' || choice == 'Y') {
            config->preserve_colors = 1;
            printf("%s\n\n", config->lang ? "   ✓ Сохранение с ANSI цветами" : "   ✓ Saving with ANSI color codes");
            return 1;
        } else if (choice == 'n' || choice == 'N') {
            config->preserve_colors = 0;
            printf("%s\n\n", config->lang ? "   ✓ Сохранение без цветов (обычный текст)" : "   ✓ Saving without color codes (plain text)");
            return 1;
        } else if (choice == 'a' || choice == 'A') {
            printf("%s\n", config->lang ? "   ✗ Отменено пользователем" : "   ✗ Aborted by user");
            return 0;
        } else {
            printf("%s ", config->lang ? "   Неверный выбор. Введите y, n или a: " : "   Invalid choice. Please enter y, n, or a: ");
        }
    }
}

void print_bytes_mode(Config *config) {
    unsigned char byte;
    char line[1024];
    FILE *urandom = NULL;
    
    #if defined(HAS_URANDOM)
        urandom = fopen("/dev/urandom", "r");
        if (!urandom && !config->output_file) {
            perror("fopen /dev/urandom");
            exit(1);
        }
    #endif
    
    if (config->output_file) {
        if (!confirm_color_save(config)) {
            if (urandom) fclose(urandom);
            return;
        }
        config->output_fp = fopen(config->output_file, "w");
        if (!config->output_fp) {
            perror("fopen output file");
            if (urandom) fclose(urandom);
            exit(1);
        }
    }
    
    if (config->switch_mode != 0 || config->color_mode == 3) {
        get_current_color(config, 1);
    }
    
    int count = 0;
    
    while (config->amount == 0 || count < config->amount) {
        line[0] = '\0';
        int pos = 0;
        int elements_in_line = 0;
        
        int remaining = config->amount - count;
        int to_output = config->line_count;
        if (config->amount > 0 && remaining < config->line_count) {
            to_output = remaining;
        }
        
        for (int i = 0; i < to_output; i++) {
            if (!read_byte(&byte, config)) break;
            pos += snprintf(line + pos, sizeof(line) - pos, "%02X", byte);
            if (i < to_output - 1) {
                pos += snprintf(line + pos, sizeof(line) - pos, " ");
            }
            count++;
            elements_in_line++;
        }
        
        if (elements_in_line == 0) break;
        
        Color current_color = {255, 255, 255};
        
        if (config->color_mode == 1) {
            current_color = config->fixed_color;
        } else if (config->color_mode == 2) {
            current_color = get_current_color(config, 0);
        } else if (config->color_mode == 3) {
            current_color = get_current_color(config, 0);
        }
        
        write_output(line, config, current_color);
        
        if (config->amount == 0 || count < config->amount) {
            sleep_ms(config->interval_ms);
        }
    }
    
    if (urandom) fclose(urandom);
    if (config->output_fp) {
        fclose(config->output_fp);
        printf(config->lang ? "\n✓ Сохранено %d элементов в: %s\n" : "\n✓ Saved %d elements to: %s\n", count, config->output_file);
    }
}

void print_printable_mode(Config *config) {
    unsigned char byte;
    char line[1024];
    FILE *urandom = NULL;
    
    #if defined(HAS_URANDOM)
        urandom = fopen("/dev/urandom", "r");
        if (!urandom && !config->output_file) {
            perror("fopen /dev/urandom");
            exit(1);
        }
    #endif
    
    if (config->output_file) {
        if (!confirm_color_save(config)) {
            if (urandom) fclose(urandom);
            return;
        }
        config->output_fp = fopen(config->output_file, "w");
        if (!config->output_fp) {
            perror("fopen output file");
            if (urandom) fclose(urandom);
            exit(1);
        }
    }
    
    if (config->switch_mode != 0 || config->color_mode == 3) {
        get_current_color(config, 1);
    }
    
    int count = 0;
    
    while (config->amount == 0 || count < config->amount) {
        line[0] = '\0';
        int pos = 0;
        int elements_in_line = 0;
        
        int remaining = config->amount - count;
        int to_output = config->line_count;
        if (config->amount > 0 && remaining < config->line_count) {
            to_output = remaining;
        }
        
        for (int i = 0; i < to_output; i++) {
            do {
                if (!read_byte(&byte, config)) break;
            } while (!isprint(byte));
            
            pos += snprintf(line + pos, sizeof(line) - pos, "%c", byte);
            if (i < to_output - 1) {
                pos += snprintf(line + pos, sizeof(line) - pos, " ");
            }
            count++;
            elements_in_line++;
        }
        
        if (elements_in_line == 0) break;
        
        Color current_color = {255, 255, 255};
        
        if (config->color_mode == 1) {
            current_color = config->fixed_color;
        } else if (config->color_mode == 2) {
            current_color = get_current_color(config, 0);
        } else if (config->color_mode == 3) {
            current_color = get_current_color(config, 0);
        }
        
        write_output(line, config, current_color);
        
        if (config->amount == 0 || count < config->amount) {
            sleep_ms(config->interval_ms);
        }
    }
    
    if (urandom) fclose(urandom);
    if (config->output_fp) {
        fclose(config->output_fp);
        printf(config->lang ? "\n✓ Сохранено %d элементов в: %s\n" : "\n✓ Saved %d elements to: %s\n", count, config->output_file);
    }
}

void print_decimals_mode(Config *config) {
    unsigned char byte;
    char line[1024];
    FILE *urandom = NULL;
    
    #if defined(HAS_URANDOM)
        urandom = fopen("/dev/urandom", "r");
        if (!urandom && !config->output_file) {
            perror("fopen /dev/urandom");
            exit(1);
        }
    #endif
    
    if (config->output_file) {
        if (!confirm_color_save(config)) {
            if (urandom) fclose(urandom);
            return;
        }
        config->output_fp = fopen(config->output_file, "w");
        if (!config->output_fp) {
            perror("fopen output file");
            if (urandom) fclose(urandom);
            exit(1);
        }
    }
    
    if (config->switch_mode != 0 || config->color_mode == 3) {
        get_current_color(config, 1);
    }
    
    int count = 0;
    
    while (config->amount == 0 || count < config->amount) {
        line[0] = '\0';
        int pos = 0;
        int elements_in_line = 0;
        
        int remaining = config->amount - count;
        int to_output = config->line_count;
        if (config->amount > 0 && remaining < config->line_count) {
            to_output = remaining;
        }
        
        for (int i = 0; i < to_output; i++) {
            if (!read_byte(&byte, config)) break;
            pos += snprintf(line + pos, sizeof(line) - pos, "%d", byte);
            if (i < to_output - 1) {
                pos += snprintf(line + pos, sizeof(line) - pos, " ");
            }
            count++;
            elements_in_line++;
        }
        
        if (elements_in_line == 0) break;
        
        Color current_color = {255, 255, 255};
        
        if (config->color_mode == 1) {
            current_color = config->fixed_color;
        } else if (config->color_mode == 2) {
            current_color = get_current_color(config, 0);
        } else if (config->color_mode == 3) {
            current_color = get_current_color(config, 0);
        }
        
        write_output(line, config, current_color);
        
        if (config->amount == 0 || count < config->amount) {
            sleep_ms(config->interval_ms);
        }
    }
    
    if (urandom) fclose(urandom);
    if (config->output_fp) {
        fclose(config->output_fp);
        printf(config->lang ? "\n✓ Сохранено %d элементов в: %s\n" : "\n✓ Saved %d elements to: %s\n", count, config->output_file);
    }
}
