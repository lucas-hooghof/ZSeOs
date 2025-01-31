#include "ata.h"

uint8_t ErrorRegisterData;
uint8_t StatusRegisterData;

bool IdentifyDrive(uint8_t Drive)
{
    uint16_t PortBase = (Drive == ATA_MASTER) ? ATA1_MASTER_PORT_START : ATA1_SLAVE_PORT_START;
    i686_outb(PortBase + ATA_DRIVE_REGISTER,Drive);

    i686_outb(PortBase + ATA_LBA_LOW_REGISTER,0);
    i686_outb(PortBase + ATA_LBA_MID_REGISTER,0);
    i686_outb(PortBase + ATA_LBA_HIGH_REGISTER,0);
    i686_outb(PortBase + ATA_SECTOR_COUNT_REGISTER,0);

    i686_outb(PortBase + ATA_COMMAND_REGISTER,0xEC);

    StatusRegisterData = i686_inb(PortBase + ATA_STATUS_REGISTER);
    if (StatusRegisterData == 0)
    {
        return false;
    }

    while(1)
    {
        StatusRegisterData = i686_inb(PortBase + ATA_STATUS_REGISTER);
        if (!(StatusRegisterData & (1 << 7)))
        {
            break;
        }
    }

    uint16_t lbaval = i686_inb(PortBase + ATA_LBA_MID_REGISTER);
    if (lbaval != 0)
    {
        printf("DRIVE: %x %x is not IDE\n",PortBase,Drive,lbaval);
        return false;
    }

    lbaval = i686_inb(PortBase + ATA_LBA_HIGH_REGISTER);
    if (lbaval != 0)    
    {
        printf("DRIVE: %x %x is not IDE\n",PortBase,Drive);
        return false;
    }
    printf("DRIVE: %x %x is IDE\n",PortBase,Drive);
    return true;
}