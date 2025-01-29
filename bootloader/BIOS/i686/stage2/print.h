#pragma once

#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include "bootinfo.h"
#include "io.h"



#define TEXT_MEMORY 0xb8000

typedef void (*putc_ptr)(char c);
typedef void (*clear_screen_ptr)();

extern putc_ptr putc;
extern clear_screen_ptr clearscreen;

void Initlize_output(uint32_t displaymode,uint8_t debug);

void puts(char* str);
void printf(const char* fmt,...);

