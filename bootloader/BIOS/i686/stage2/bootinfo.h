#pragma once

#include <stdint.h>

typedef enum 
{
    TEXT=0,
    VGA=1,
    VESA=2
}DISPLAY;

typedef struct
{
    uint16_t xsize;
    uint16_t ysize;
}TEXT_MODE_DISPLAY_DATA;


typedef struct 
{
    uint8_t DisplayMode;
    uint16_t MemoryMapLocation;
    uintptr_t DisplayData;
}__attribute__((packed))bootinfo_t;