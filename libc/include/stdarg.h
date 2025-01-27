#pragma once
#include <stdint.h>
typedef uintptr_t* va_list;

#define va_start(list,arg) (list=(uintptr_t*)arg+sizeof(arg))
#define va_arg(list,type) *(type*)list++;
 