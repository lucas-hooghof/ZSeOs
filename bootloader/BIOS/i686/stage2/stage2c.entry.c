#include <stdint.h>
#include "bootinfo.h"
#include "print.h"
#include "io.h"

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

    printf("");
    for (;;)
    {}
}