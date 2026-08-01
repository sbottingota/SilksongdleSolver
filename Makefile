CC=gcc

build/main: src/main.c
	$(CC) $(CFLAGS) src/main.c -o build/main
