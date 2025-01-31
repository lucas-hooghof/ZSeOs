#include <stdint.h>
#include "bootinfo.h"
#include "print.h"
#include "io.h"
#include "memory.h"
#include "ACPI.h"
#include "ata.h"

void _cstart()
{
    bootinfo_t* info = (bootinfo_t*)0x00000500;
    Initlize_output(info->DisplayMode,1);
    if (info->DisplayMode == 0)
    {
        //Check if VGA controller is in use
        uint8_t retrace = i686_inb(0x3DA);
        if ((retrace & 0x08) != 0)
        {
            puts("VGA controller is possible/useable\n");
        }
    }

    printf("Bootinfo Location: %llx -> Entries:\n",info);
    printf("    Memory Map Location: %x\n",info->MemoryMapLocation);
    printf("    Block Count: %hhu\n",info->MemoryMapBlockCount);

    printf("Printing Memory:\n");

    for (int i = 0; i < info->MemoryMapBlockCount; i++)
    {
        MemoryBlock* block = (MemoryBlock*)(info->MemoryMapLocation + (i * sizeof(MemoryBlock)));
        printf("%i: Base=%llx,Length=%llx,Type=%u\n",i,block->base,block->length,block->regiontype);
    }

    int ACPIVer = GetACPIRevision();
    if (ACPIVer == -1)
    {
        printf("Failed to get ACPI");
    }

    printf("ACPI version: %i\n",ACPIVer);
    PrintACPITables();

    //printf("Check for IDE devices\n");
    //IdentifyDrive(ATA_MASTER);
    //IdentifyDrive(ATA_SLAVE);

    
    for (;;)
    {}
}