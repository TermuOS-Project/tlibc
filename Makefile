CC     ?= gcc
AR     ?= ar
CFLAGS ?= -ffreestanding -fno-stack-protector -fno-builtin -fno-pic \
          -mno-sse -mno-sse2 -O2 -Wall -Wextra -Iinclude

SRCS := \
	src/string/strlen.c \
	src/string/strcmp.c \
	src/string/strncmp.c \
	src/string/strcpy.c \
	src/string/strncpy.c \
	src/string/memcpy.c \
	src/string/memmove.c \
	src/string/memset.c \
	src/string/memcmp.c \
	src/stdio/putchar.c \
	src/stdio/puts.c \
	src/stdio/printf.c \
	src/stdlib/exit.c \
	src/stdlib/malloc.c \
	src/sys/syscall.c \
	src/unistd/unistd.c

OBJS := $(SRCS:.c=.o)
LIB  := libtlibc.a

.PHONY: all clean

all: $(LIB)

src/string/%.o: src/string/%.c include/string.h include/stddef.h
	$(CC) $(CFLAGS) -c $< -o $@

src/stdio/%.o: src/stdio/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/stdlib/%.o: src/stdlib/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/sys/%.o: src/sys/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/unistd/%.o: src/unistd/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) rcs $@ $(OBJS)
	@echo "built $@"

clean:
	rm -f $(OBJS) $(LIB)
