#include "PCI.h"

//uint16_t PCIConfigReadWord(uint8_t bus,uint8_t slot,uint8_t func,uint8_t offset)
//{
//    uint32_t address;
//    uint32_t lbus  = (uint32_t)bus;
//    uint32_t lslot = (uint32_t)slot;
//    uint32_t lfunc = (uint32_t)func;
//    uint16_t tmp = 0;
//  
//    address = (uint32_t)((lbus << 16) | (lslot << 11) |
//              (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
//  
//
//    i686_outl(0xCF8, address);
//
//    tmp = (uint16_t)((i686_inl(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
//    return tmp;
//}

void EnumaratePCI()
{
//    for (uint16_t bus = 0; bus < PCI_MAX_BUSES; bus++)
//    {
//        for (uint8_t slot = 0; slot < PCI_MAX_DEVICES; slot++)
//        {
//            for (uint8_t function = 0; function < PCI_MAX_FUNCTIONS; function++)
//            {
//
//            }
//        }
//    }
}