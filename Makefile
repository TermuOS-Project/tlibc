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
	src/string/memcmp.c

OBJS := $(SRCS:.c=.o)
LIB  := libtlibc.a

.PHONY: all clean

all: $(LIB)

src/string/%.o: src/string/%.c include/string.h include/stddef.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	$(AR) rcs $@ $(OBJS)
	@echo "built $@"

clean:
	rm -f $(OBJS) $(LIB)