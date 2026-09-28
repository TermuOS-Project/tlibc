#pragma once

#include <stddef.h>

long write(int fd, const void *buf, unsigned long n);
void _exit(int code);