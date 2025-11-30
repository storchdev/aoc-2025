CC = gcc

# Normal and release flags
CFLAGS = -Wall -Wextra -g -fsanitize=address,undefined
RELEASE_FLAGS = -Wall -Wextra -O2

# Linker flags (system libs)
LDFLAGS = -lm -lgmp -lglib-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include

COMMON = common.c

# All C files directly under day directories, e.g. day0/main.c, day1/part1.c
DAY_SRCS := $(wildcard day*/*.c)

# Turn dayN/foo.c -> dayN/foo.out
DAY_BINS := $(DAY_SRCS:.c=.out)

.PHONY: all clean release

all: $(DAY_BINS)

# Generic rule: any .out next to a .c
%.out: %.c $(COMMON)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

release:
	$(MAKE) clean
	$(MAKE) CLFAGS="$(RELEASE_FLAGS)"

clean:
	rm -f $(DAY_BINS)





