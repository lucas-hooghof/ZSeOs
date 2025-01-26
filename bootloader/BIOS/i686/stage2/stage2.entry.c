#include <stdint.h>
#include "bootinfo.h"
#include "print.h"
void _cstart(bootinfo_t* bootinfo)
{
    Initlize_output(bootinfo);

    putc('B');
    putc('C');
}