#include "randbytegen.h"

static Color colors[] = {
    {255, 0, 0},     // RED
    {0, 255, 0},     // GREEN
    {0, 0, 255},     // BLUE
    {255, 255, 0},   // YELLOW
    {255, 0, 255},   // MAGENTA
    {0, 255, 255},   // CYAN
    {255, 165, 0},   // ORANGE
    {128, 0, 128},   // PURPLE
    {255, 192, 203}, // PINK
    {255, 255, 255}  // WHITE
};

static int color_count = sizeof(colors) / sizeof(colors[0]);

int parse_hex_color(const char *hex, Color *color) {
    if (hex[0] == '#') hex++;
    if (strlen(hex) != 6) return 0;
    
    for (int i = 0; i < 6; i++) {
        if (!isxdigit(hex[i])) return 0;
    }
    
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
    
    for (int i = 0; upper[i]; i++) {
        upper[i] = toupper(upper[i]);
    }
    
    const char *names[] = {"RED", "GREEN", "BLUE", "YELLOW", "MAGENTA", 
                           "CYAN", "ORANGE", "PURPLE", "PINK", "WHITE"};
    
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

void print_colored(const char *text, Color color) {
    printf("\033[38;2;%d;%d;%dm%s\033[0m\n", color.r, color.g, color.b, text);
}
