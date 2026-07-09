CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = randbytegen
SOURCES = main.c colors.c output.c
OBJECTS = $(SOURCES:.c=.o)
PREFIX = /usr/local/bin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c randbytegen.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	@echo "Installing $(TARGET) to $(PREFIX)"
	@if [ ! -d "$(PREFIX)" ]; then \
		echo "Creating directory $(PREFIX)"; \
		sudo mkdir -p $(PREFIX); \
	fi
	sudo cp $(TARGET) $(PREFIX)/
	sudo chmod 755 $(PREFIX)/$(TARGET)
	@echo "Done! Run '$(TARGET) --help' to get started"

uninstall:
	@echo "Removing $(TARGET) from $(PREFIX)"
	@if [ -f "$(PREFIX)/$(TARGET)" ]; then \
		sudo rm -f $(PREFIX)/$(TARGET); \
		echo "Removed"; \
	else \
		echo "File not found"; \
	fi

reinstall: clean install

run: $(TARGET)
	./$(TARGET) -b -l 5 -a 20 -c RANDOM

run-gradient: $(TARGET)
	./$(TARGET) -b -l 5 -a 20 -c blue:red

run-decimal: $(TARGET)
	./$(TARGET) -d -l 3 -a 9 -c GREEN -i 500

run-printable: $(TARGET)
	./$(TARGET) -p -l 4 -a 8 -c #FF69B4 -i 1000

run-file: $(TARGET)
	./$(TARGET) -b -l 5 -a 20 -c RANDOM -o output.txt

run-sysinfo: $(TARGET)
	./$(TARGET) --sysinfo

run-version: $(TARGET)
	./$(TARGET) -v

run-help: $(TARGET)
	./$(TARGET) -h

help:
	@echo "Available commands:"
	@echo "  make              - build randbytegen"
	@echo "  make clean        - remove object files and binary"
	@echo "  make install      - install to /usr/local/bin (requires sudo)"
	@echo "  make uninstall    - remove from /usr/local/bin"
	@echo "  make reinstall    - clean + install"
	@echo "  make run          - test run (HEX + random colors)"
	@echo "  make run-gradient - test gradient"
	@echo "  make run-decimal  - test decimal mode"
	@echo "  make run-printable - test printable mode"
	@echo "  make run-file     - test file output"
	@echo "  make run-sysinfo  - show system info"
	@echo "  make run-version  - show version"
	@echo "  make run-help     - show help"
	@echo "  make help         - this help"

.PHONY: all clean install uninstall reinstall run run-gradient run-decimal run-printable run-file run-sysinfo run-version run-help help