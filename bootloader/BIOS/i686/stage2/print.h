#pragma once

#include <stdint.h>

#include "bootinfo.h"

#define TEXT_MEMORY 0xb8000

typedef void (*putc_ptr)(char c);
typedef void (*clear_screen_ptr)();

extern putc_ptr putc;
extern clear_screen_ptr clearscreen;

void Initlize_output(bootinfo_t* bootinfo);