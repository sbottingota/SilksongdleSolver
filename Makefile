CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic --std=c23

.DEFAULT_GOAL := build/main
.PHONY: clean

SRCS=$(wildcard src/*)

build/main: $(SRCS)
	@if [ ! -d "build" ]; then mkdir build; fi
	$(CC) $(CFLAGS) src/*.c -o build/main -lm 

debug: build/main_debug

build/main_debug: $(SRCS)
	@if [ ! -d "build" ]; then mkdir build; fi
	$(CC) $(CFLAGS) -g -O0 -DDEBUG_INFO src/*.c -o build/main_debug -lm 

clean:
	rm -r build/
