#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../CCs/CCS.h"


#define MBR_SIGNATURE 0xAA55
#define MBR_BOOTSTRAP_CODE_SIZE 440

//MBR
typedef struct 
{
    uint8_t BootIndicator; // 1 if bootable 0 if non bootable
    uint8_t StartCHS[3];    // Cylinder Head Sector
    uint8_t OsType;         // Used to identify drives
    uint8_t EndCHS[3];
    uint32_t StartLBA;      //Logical Block addressing. This is what we will be using
    uint32_t SizeLBA;  
}__attribute__((packed))MBR_PARTITION;

typedef struct 
{
    uint8_t BootStrapCode[MBR_BOOTSTRAP_CODE_SIZE]; //This will be read from the stage0 file
    uint32_t UniqueMBRSig;
    uint16_t unknown;
    MBR_PARTITION parttable[4];
    uint16_t Signature;         //0xAA55
}__attribute__((packed))MBR;

// These will be used to get identifyers
typedef enum 
{
    UEFI_SYSTEM_PART= 0xEF,
    GPT_PROTECTIVE_PART=0xEE,    //This is used to fake a partition to cover the entire disk to protect it
    ZSEOS_BOOT_DRIVE=0xCC,
    ZSEOS_ROOT_DRIVE=0xBC,
    ZSEOS_SWAP_DRIVE=0xDF,
}OSTYPE;

extern MBR_PARTITION mbrparttable[4]; // We do this so we can hardcore the partition table

bool BIOS_WriteMBRToFile(FILE* file,char* stage0code);