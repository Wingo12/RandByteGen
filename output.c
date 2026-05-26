#include "randbytegen.h"

static void sleep_ms(int ms) {
    struct timespec req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&req, NULL);
}

void print_bytes_mode(Config *config) {
    unsigned char byte;
    char output[4];
    FILE *urandom = fopen("/dev/urandom", "r");
    
    if (!urandom) {
        perror("fopen");
        exit(1);
    }
    
    while (1) {
        if (fread(&byte, 1, 1, urandom) != 1) break;
        snprintf(output, sizeof(output), "%02X", byte);
        
        if (config->color_mode == 1) {
            print_colored(output, config->fixed_color);
        } else if (config->color_mode == 2) {
            Color rand_color;
            random_color(&rand_color);
            print_colored(output, rand_color);
        } else {
            printf("%s\n", output);
        }
        
        fflush(stdout);
        sleep_ms(config->interval_ms);
    }
    
    fclose(urandom);
}

void print_printable_mode(Config *config) {
    unsigned char byte;
    char output[2];
    FILE *urandom = fopen("/dev/urandom", "r");
    
    if (!urandom) {
        perror("fopen");
        exit(1);
    }
    
    while (1) {
        do {
            if (fread(&byte, 1, 1, urandom) != 1) return;
        } while (!isprint(byte));
        
        snprintf(output, sizeof(output), "%c", byte);
        
        if (config->color_mode == 1) {
            print_colored(output, config->fixed_color);
        } else if (config->color_mode == 2) {
            Color rand_color;
            random_color(&rand_color);
            print_colored(output, rand_color);
        } else {
            printf("%s\n", output);
        }
        
        fflush(stdout);
        sleep_ms(config->interval_ms);
    }
    
    fclose(urandom);
}

void print_decimals_mode(Config *config) {
    unsigned char byte;
    char line[1024];
    FILE *urandom = fopen("/dev/urandom", "r");
    
    if (!urandom) {
        perror("fopen");
        exit(1);
    }
    
    while (1) {
        line[0] = '\0';
        int pos = 0;
        
        for (int i = 0; i < config->count_per_line; i++) {
            if (fread(&byte, 1, 1, urandom) != 1) break;
            pos += snprintf(line + pos, sizeof(line) - pos, "%d", byte);
            if (i < config->count_per_line - 1) {
                pos += snprintf(line + pos, sizeof(line) - pos, " ");
            }
        }
        
        if (config->color_mode == 1) {
            print_colored(line, config->fixed_color);
        } else if (config->color_mode == 2) {
            Color rand_color;
            random_color(&rand_color);
            print_colored(line, rand_color);
        } else {
            printf("%s\n", line);
        }
        
        fflush(stdout);
        sleep_ms(config->interval_ms);
    }
    
    fclose(urandom);
}
