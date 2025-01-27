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
    if (c == '\n')
    {
        x_text_mode = 0;
        y_text_mode++;
        return;
    }

    int8_t* location = (int8_t*)(TEXT_MEMORY);
    location[2 * (y_text_mode * x_max_text_mode + x_text_mode)] = c;


    x_text_mode++;
    if (x_text_mode >= x_max_text_mode)
    {
        x_text_mode = 0;
        y_text_mode++;
    }
}

void puts(char* str)
{
    while(*str)
    {
        putc(*str);
        str++;
    }
}



void Initlize_output(uint32_t displaymode)
{
    if (displaymode == TEXT)
    {     
        x_max_text_mode = 80;
        y_max_text_mode = 25;

        putc = putc_text_mode;
    }
}

#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1

void printf(const char* fmt,...)
{
    //Use varible arguments
    va_list list;
    va_start(list,fmt);
    (void)list;

    int state = PRINTF_STATE_NORMAL;

    while (*fmt)
    {
        switch(state)
        {
            case PRINTF_STATE_NORMAL:
            {
                switch (*fmt)
                {
                case '%':
                    state = PRINTF_STATE_LENGTH;
                    break;
                
                default:
                    putc(*fmt);
                    break;
                }
                break;
            }

            case PRINTF_STATE_LENGTH:
            {
                switch (*fmt)
                {
                    case '%':
                        putc('%');
                        state = PRINTF_STATE_NORMAL;
                        break;
                }
                break;
            }
            
        }
        fmt++;
    }
}