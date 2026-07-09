#include "randbytegen.h"

static Color colors[] = {
    {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
    {255, 255, 0}, {255, 0, 255}, {0, 255, 255},
    {255, 165, 0}, {128, 0, 128}, {255, 192, 203}, {255, 255, 255}
};
static int color_count = sizeof(colors) / sizeof(colors[0]);

int parse_hex_color(const char *hex, Color *color) {
    if (hex[0] == '#') hex++;
    if (strlen(hex) != 6) return 0;
    for (int i = 0; i < 6; i++) if (!isxdigit(hex[i])) return 0;
    unsigned long rgb = strtoul(hex, NULL, 16);
    color->r = (rgb >> 16) & 0xFF;
    color->g = (rgb >> 8) & 0xFF;
    color->b = rgb & 0xFF;
    return 1;
}

int parse_color_name(const char *name, Color *color) {
    char upper[32];
    strncpy(upper, name, sizeof(upper) - 1);
    upper[sizeof(upper) - 1] = '\0';
    for (int i = 0; upper[i]; i++) upper[i] = toupper(upper[i]);
    const char *names[] = {"RED", "GREEN", "BLUE", "YELLOW", "MAGENTA", "CYAN", "ORANGE", "PURPLE", "PINK", "WHITE"};
    for (int i = 0; i < color_count; i++) {
        if (strcmp(upper, names[i]) == 0) {
            *color = colors[i];
            return 1;
        }
    }
    return 0;
}

void random_color(Color *color) {
    color->r = rand() % 256;
    color->g = rand() % 256;
    color->b = rand() % 256;
}

Color lerp_color(Color start, Color end, float t) {
    Color result;
    result.r = (int)(start.r + (end.r - start.r) * t);
    result.g = (int)(start.g + (end.g - start.g) * t);
    result.b = (int)(start.b + (end.b - start.b) * t);
    return result;
}

char* color_to_ansi(Color color) {
    static char buffer[32];
    snprintf(buffer, sizeof(buffer), "\033[38;2;%d;%d;%dm", color.r, color.g, color.b);
    return buffer;
}

void print_colored(const char *text, Color color) {
    printf("\033[38;2;%d;%d;%dm%s\033[0m\n", color.r, color.g, color.b, text);
}

Color get_current_color(Config *config, int reset) {
    static Color current_color = {255, 255, 255};
    static int counter = 0;
    static time_t last_switch = 0;
    static int step = 0;
    static int line_counter = 0;
    
    if (reset) {
        counter = 0;
        last_switch = time(NULL);
        step = 0;
        line_counter = 0;
        if (config->color_mode == 3) {
            current_color = config->color_start;
        } else if (config->color_mode == 2) {
            random_color(&current_color);
        }
        return current_color;
    }
    
    line_counter++;
    
    // === ГРАДИЕНТ (с поддержкой switch) ===
    if (config->color_mode == 3) {
        if (config->switch_mode != 0) {
            int should_reset = 0;
            switch (config->switch_mode) {
                case 1:
                    if (time(NULL) - last_switch >= config->switch_value) {
                        should_reset = 1;
                        last_switch = time(NULL);
                    }
                    break;
                case 2:
                    if (line_counter >= config->switch_value) {
                        should_reset = 1;
                        line_counter = 0;
                    }
                    break;
                case 3:
                    if (counter >= config->switch_value) {
                        should_reset = 1;
                        counter = 0;
                    }
                    break;
            }
            if (should_reset) {
                step = 0;
                current_color = config->color_start;
            }
        }
        step++;
        float t = (float)step / config->gradient_steps;
        if (t > 1.0) t = 1.0;
        current_color = lerp_color(config->color_start, config->color_end, t);
        if (step >= config->gradient_steps) step = 0;
        return current_color;
    }
    
    // === RANDOM ===
    if (config->color_mode == 2) {
        if (config->switch_mode == 0) {
            random_color(&current_color);
            return current_color;
        }
        int should_switch = 0;
        switch (config->switch_mode) {
            case 1:
                if (time(NULL) - last_switch >= config->switch_value) {
                    should_switch = 1;
                    last_switch = time(NULL);
                }
                break;
            case 2:
                if (line_counter >= config->switch_value) {
                    should_switch = 1;
                    line_counter = 0;
                }
                break;
            case 3:
                counter++;
                if (counter >= config->switch_value) {
                    should_switch = 1;
                    counter = 0;
                }
                break;
        }
        if (should_switch) {
            random_color(&current_color);
        }
        return current_color;
    }
    
    // === FIXED ===
    if (config->color_mode == 1) {
        return config->fixed_color;
    }
    
    // === NO COLOR ===
    return (Color){255, 255, 255};
}
