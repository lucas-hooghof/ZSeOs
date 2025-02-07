ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
    . = 0x8000;

    .text : {
        *(.early)
        *(.text)
        *(.text.*)
    }

    .rodata : 
    {
        *(.rodata)
        *(.rodata.*)
    }

    .data :
    {
        *(.data)
        *(.data.*)
    }

    .bss :
    {
        *(.bss)
        *(.data.*)
    }
}