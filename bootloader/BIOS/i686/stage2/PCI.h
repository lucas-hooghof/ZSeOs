#pragma once

#include <stdint.h>
#include "io.h"

#define PCI_VENDOR_ID                           0x00
#define PCI_DEVICE_ID                           0x02
#define PCI_COMMAND                             0x04
#define PCI_STATUS                              0x06
#define PCI_REVISION_AND_PROGRAM_INTERFACE      0x08
#define PCI_SUBCLASS_AND_CLASS_CODE             0x0A
#define PCI_CACHE_LINE_SIZE_AND_LATENCY_TIMER   0x0C
#define PCI_HEADER_TYPE_BIST                    0x0E

#define PCI_MAX_BUSES                           256
#define PCI_MAX_DEVICES                         32
#define PCI_MAX_FUNCTIONS                       8
typedef struct 
{
    uint16_t VendorID;
    uint16_t DeviceID;
    uint16_t Command;
    uint16_t Status;
    uint8_t  RevisionID;
    uint8_t  ProgramInterface;
    uint8_t  SubClass;
    uint8_t  ClassCode;
    uint8_t  CacheLineSize;
    uint8_t  LatencyTimer;
    uint8_t  HeaderType;
    uint8_t  BIST;
}__attribute__((packed))CommonHeader;

uint16_t PCIConfigReadWord(uint8_t bus,uint8_t slot,uint8_t func,uint8_t offset);

void EnumaratePCI();

