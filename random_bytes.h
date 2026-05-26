#ifndef RANDOM_BYTES_H
#define RANDOM_BYTES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    int r, g, b;
} Color;

typedef struct {
    int mode;           // 0: bytes, 1: printable, 2: decimals
    int count_per_line;
    int color_mode;     // 0: нет, 1: фикс, 2: random
    int interval_ms;
    Color fixed_color;
} Config;

// colors.c
int parse_hex_color(const char *hex, Color *color);
int parse_color_name(const char *name, Color *color);
void random_color(Color *color);
void print_colored(const char *text, Color color);

// output.c
void print_bytes_mode(Config *config);
void print_printable_mode(Config *config);
void print_decimals_mode(Config *config);

// main.c
void print_usage(const char *progname);
Config parse_arguments(int argc, char *argv[]);

#endif
