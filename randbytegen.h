#ifndef RANDBYTEGEN_H
#define RANDBYTEGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <langinfo.h>

#ifdef _WIN32
    #define OS_WINDOWS 1
    #include <windows.h>
    #include <io.h>
    #define OS_NAME "Windows"
#elif __APPLE__
    #define OS_MACOS 1
    #include <sys/param.h>
    #include <sys/sysctl.h>
    #define OS_NAME "macOS"
#elif __linux__
    #define OS_LINUX 1
    #include <sys/utsname.h>
    #define OS_NAME "Linux"
#else
    #define OS_UNKNOWN 1
    #define OS_NAME "Unknown"
#endif

#if defined(OS_LINUX) || defined(OS_MACOS)
    #define HAS_URANDOM 1
#endif

#define VERSION "0.4"
#define AUTHOR "Wingo123"
#define GITHUB "https://github.com/Wingo12"

typedef struct {
    int r, g, b;
} Color;

typedef struct {
    int mode;
    int line_count;
    int amount;
    int color_mode;
    int interval_ms;
    Color fixed_color;
    Color color_start;
    Color color_end;
    int gradient_steps;
    int switch_mode;
    int switch_value;
    int switch_counter;
    time_t switch_time;
    char *output_file;
    FILE *output_fp;
    int preserve_colors;
    int lang;
    int os_type;
    char os_name[32];
} Config;

int parse_hex_color(const char *hex, Color *color);
int parse_color_name(const char *name, Color *color);
void random_color(Color *color);
void print_colored(const char *text, Color color);
Color get_current_color(Config *config, int reset);
Color lerp_color(Color start, Color end, float t);
char* color_to_ansi(Color color);

void print_bytes_mode(Config *config);
void print_printable_mode(Config *config);
void print_decimals_mode(Config *config);
int confirm_color_save(Config *config);

void print_usage(const Config *config);
void print_version(const Config *config);
void print_system_info(const Config *config);
Config parse_arguments(int argc, char *argv[]);
int detect_language();
void detect_os(Config *config);
int check_urandom();
unsigned char get_random_byte();
const char* get_message(Config *config, int id);

#endif
