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
    uint32_t DisplayMode;
    uint32_t MemoryMapLocation;
    uint8_t MemoryMapBlockCount;
}__attribute__((packed))bootinfo_t;