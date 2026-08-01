CC=gcc
CFLAGS = -Wall -Wextra -Wpedantic --std=c23

.PHONY: clean

SRCS=$(wildcard src/*)

build/main: $(SRCS)
	$(CC) $(CFLAGS) src/*.c -o build/main

clean:
	rm -f build/*
