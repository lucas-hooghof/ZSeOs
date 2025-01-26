#include "print.h"

putc_ptr putc;
clear_screen_ptr clearscreen;

//Private printing function
//Text mode
uint16_t x_text_mode = 0;
uint16_t y_text_mode = 0;

uint16_t x_max_text_mode = 0;
uint16_t y_max_text_mode = 0;
void putc_text_mode(char c)
{
    int8_t* location = (int8_t*)(TEXT_MEMORY + x_text_mode + (y_text_mode * x_max_text_mode));
    *location = c;

    x_text_mode++;
    if (x_text_mode >= x_max_text_mode)
    {
        *location = 'Z';

        x_text_mode = 0;
        y_text_mode++;
    }
}



void Initlize_output(bootinfo_t* bootinfo)
{
    if (bootinfo->DisplayMode == 0)
    {
        TEXT_MODE_DISPLAY_DATA* display_data = (TEXT_MODE_DISPLAY_DATA*)bootinfo->DisplayData;
        x_max_text_mode = display_data->xsize;
        y_max_text_mode = display_data->ysize;

        putc = putc_text_mode;
    }
}