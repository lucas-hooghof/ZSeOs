#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "memory.h"


typedef struct 
{
    char signature[8];
    uint8_t checksum;
    char OEMid[6];
    uint8_t revision;
    uint32_t RSDTAddress;
}__attribute__((packed))RSDP1;

typedef struct 
{
    //RSDT STUFF
    char signature[8];
    uint8_t checksum;
    char OEMid[6];
    uint8_t revision;
    uint32_t RSDTAddress;

    //XSDT Stuff
    uint32_t Length;
    uint64_t XsdtAddress;
    uint8_t ExtendedChecksum;
    uint8_t reserved[3];
}__attribute__((packed))RSDP2;

typedef struct {
  char Signature[4];
  uint32_t Length;
  uint8_t Revision;
  uint8_t Checksum;
  char OEMID[6];
  char OEMTableID[8];
  uint32_t OEMRevision;
  uint32_t CreatorID;
  uint32_t CreatorRevision;
}__attribute__((packed))SDTHeader;


extern void* rsdp;
extern int ACPIVersion;

int GetACPIRevision();

void PrintACPITables();
SDTHeader* CheckForACPITable(char* signature);