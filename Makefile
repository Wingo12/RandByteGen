CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = randbytegen
SOURCES = main.c colors.c output.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c random_bytes.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET) -b -c RANDOM

run-decimal: $(TARGET)
	./$(TARGET) -d 5 -c GREEN -i 500

run-printable: $(TARGET)
	./$(TARGET) -p -c #FF69B4 -i 1000

help:
	@echo "Commands:"
	@echo "  make              - build"
	@echo "  make clean        - remove object files and binary"
	@echo "  make run          - test run (HEX + random colors)"
	@echo "  make run-decimal  - test decimal mode"
	@echo "  make run-printable - test printable mode"
	@echo "  make help         - this help"

.PHONY: all clean run run-decimal run-printable help
