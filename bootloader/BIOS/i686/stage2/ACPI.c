#include "ACPI.h"
#include "print.h"

BDA* biosDataArea = (BDA*)(0x400);
char RSDPSignature[8] = "RSD PTR ";

void* rsdp;
int ACPIVersion;

int GetACPIRevision()
{
    // Check for RSDP at EBDA
    uint32_t EBDAAddress = biosDataArea->EBDAAddress << 4;

    for (uint32_t CurrentAddress = EBDAAddress; CurrentAddress < EBDAAddress + 1024; CurrentAddress+=8)
    {
        const void* ebdaptr = (const void*)CurrentAddress;
        if (!memcmp(ebdaptr,RSDPSignature,8))
        {
            printf("Found RSDP %lp\n",ebdaptr);
            RSDP1* rsdp_ = (RSDP1*)ebdaptr;
            rsdp = (void*)rsdp_;
            ACPIVersion = rsdp_->revision;
            return rsdp_->revision;
        }
    }

    //If not in the EBDA check a bios memory rom location
    for (uint32_t CurrentAddress = 0x000E0000; CurrentAddress < 0x000FFFFF; CurrentAddress+=8)
    {
        const void* ebdaptr = (const void*)CurrentAddress;
        if (!memcmp(ebdaptr,RSDPSignature,8))
        {
            printf("Found RSDP at: %lp\n",ebdaptr);
            RSDP1* rsdp_ = (RSDP1*)ebdaptr;
            rsdp = (void*)rsdp_;
            ACPIVersion = rsdp_->revision;
            return rsdp_->revision;
        }
    }

    return -1;
}

void PrintACPITables()
{
    if (ACPIVersion == 0)
    {
        RSDP1* rsdp1 = rsdp;
        SDTHeader* rsdt = (SDTHeader*)(rsdp1->RSDTAddress);
        int entries = (rsdt->Length - sizeof(SDTHeader)) / 8;
        for (int e = 0; e < entries; e++)
        {
            SDTHeader* tableHeader = (SDTHeader*)*(uint32_t*)((uint32_t)rsdt + sizeof(SDTHeader) + (e * 8));
            for (int i = 0; i < 4; i++)
            {
                putc(tableHeader->Signature[i]);
            }
            putc('\n');
        }
    }
    else if (ACPIVersion >= 1)
    {
        RSDP2* rsdp2 = rsdp;
        SDTHeader* xsdt = (SDTHeader*)(uint32_t)(rsdp2->XsdtAddress);
        int entries = (xsdt->Length - sizeof(SDTHeader)) / 8;
        for (int e = 0; e < entries; e++)
        {
            SDTHeader* tableHeader = (SDTHeader*)*(uint32_t*)((uint32_t)xsdt + sizeof(SDTHeader) + (e * 8));
            for (int i = 0; i < 4; i++)
            {
                putc(tableHeader->Signature[i]);
            }
            putc('\n');
        }
    }
}

SDTHeader* CheckForACPITable(char* signature)
{
    if (ACPIVersion == 0)
    {
        RSDP1* rsdp1 = rsdp;
        SDTHeader* rsdt = (SDTHeader*)(rsdp1->RSDTAddress);
        int entries = (rsdt->Length - sizeof(SDTHeader)) / 8;
        for (int e = 0; e < entries; e++)
        {
            SDTHeader* tableHeader = (SDTHeader*)*(uint32_t*)((uint32_t)rsdt + sizeof(SDTHeader) + (e * 8));
            if(!memcmp(tableHeader->Signature,signature,4))
            {
                return tableHeader;
            }
        }
    }
    else if (ACPIVersion >= 1)
    {
        RSDP2* rsdp2 = rsdp;
        SDTHeader* xsdt = (SDTHeader*)(uint32_t)(rsdp2->XsdtAddress);
        int entries = (xsdt->Length - sizeof(SDTHeader)) / 8;
        for (int e = 0; e < entries; e++)
        {
            SDTHeader* tableHeader = (SDTHeader*)*(uint32_t*)((uint32_t)xsdt + sizeof(SDTHeader) + (e * 8));
            if(!memcmp(tableHeader->Signature,signature,4))
            {
                return tableHeader;
            }
        }
    }
    return 0;
}