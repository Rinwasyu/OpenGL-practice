.PHONY: all clean

CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -O3
LDFLAGS := -lglfw -lGL -lGLU

all: triangle

clean:
	rm triangle

triangle: triangle.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<