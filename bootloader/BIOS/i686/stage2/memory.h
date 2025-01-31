#pragma once

#include <stdint.h>

typedef struct 
{
    uint64_t base;
    uint64_t length;
    uint32_t regiontype;
    uint32_t ACPI;
}MemoryBlock;

typedef struct 
{
    uint16_t Comports[4];
    uint16_t LPTPorts[3];
    uint16_t EBDAAddress;
    uint16_t Bitflags;
    uint16_t MemoryBeforeEBDA;
    uint16_t KeystateFlags;
    uint8_t Keybuffer[32];
    uint8_t DisplayMode;
    uint16_t TextModeCollums;
    uint16_t baseIOPortForVideo;
    uint16_t IRQTicksSinceBoot;
    uint8_t  CountOfDriveDetected;
    uint16_t KeyboardBufferStart;
    uint16_t KeybufferEnd;
    uint8_t KeyboardLastShift;
}__attribute__((packed)) BDA;

extern BDA* biosDataArea;

#define MEMORY_BLOCK_USEABLE        1
#define MEMORY_BLOCK_RESERVED       2
#define MEMORY_BLOCK_ACPI_RECLAIM   3
#define MEMORY_BLOCK_ACPI_NVS       4
#define MEMORY_BLOCK_BAD_MEMORY     5

int memcmp(const void* ptr1,const void* ptr2,uint32_t size);