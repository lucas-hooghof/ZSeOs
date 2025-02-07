#include "print.h"
#include <stdarg.h>

putc_ptr putc;
clear_screen_ptr clearscreen;


uint8_t debugenabled;
//Private printing function
//Text mode
uint16_t x_text_mode = 0;
uint16_t y_text_mode = 0;

uint16_t x_max_text_mode = 0;
uint16_t y_max_text_mode = 0;
void putc_text_mode(char c)
{
    if (debugenabled)
    {
        i686_outb(0xE9,c);
    }
    if (c == '\n')
    {
        x_text_mode = 0;
        y_text_mode++;
        return;
    }
    else if (c == '\t')
    {
        for (int i = 0; i < 4; i++)
        {
            putc(' ');
        }
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



void Initlize_output(uint32_t displaymode,uint8_t debug)
{
    debugenabled = debug;
    if (displaymode == TEXT)
    {     
        x_max_text_mode = 80;
        y_max_text_mode = 25;

        putc = putc_text_mode;
    }
}
#define PRINTF_LENGTH_NORMAL        0   
#define PRINTF_LENGTH_LONG          1
#define PRINTF_LENGTH_LONG_LONG     2
#define PRINTF_LENGTH_SHORT         3
#define PRINTF_LENGTH_SHORT_SHORT   4

#define PRINTF_STATE_NORMAL         0
#define PRINTF_STATE_LENGTH         1
#define PRINTF_STATE_LENGTH_SHORT   2
#define PRINTF_STATE_LENGTH_LONG    3
#define PRINTF_STATE_SPEC           4


static const char* hexchars = "0123456789abcdef";
void printf_unsigned(unsigned long long number, int radix) {
    char buffer[32];
    int pos = 0;
    if (radix == 16)
    {
        puts("0x");
    }
    do {
        unsigned long long rem = number % radix;
        number /= radix;
        buffer[pos++] = hexchars[rem];
    } while (number > 0);
    while (--pos >= 0) {
        putc(buffer[pos]);
    }
}

void printf_signed(long long number, int radix)
{
    if (number < 0)
    {
        putc('-');
        printf_unsigned(-number, radix);
    }
    else printf_unsigned(number, radix);
}

void printf(const char* fmt,...)
{
    //Use varible argument
    va_list args;
    va_start(args,fmt);

    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_NORMAL;

    bool number = false;
    bool sign = false;
    uint8_t radix = 10;

    (void)length;
    (void)number;
    (void)sign;
    (void)radix;

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
                    case 'l':
                        state = PRINTF_STATE_LENGTH_LONG;
                        length = PRINTF_LENGTH_LONG;
                        break;
                    case 'h':
                        state = PRINTF_STATE_LENGTH_SHORT;
                        length = PRINTF_LENGTH_SHORT;
                        break;
                    default: goto PRINTF_SPEC;
                }

                break;
            }

            case PRINTF_STATE_LENGTH_LONG:
            {
                if (*fmt == 'l')
                {
                    state = PRINTF_STATE_SPEC;
                    length = PRINTF_LENGTH_LONG_LONG;
                }
                else goto PRINTF_SPEC;
                break;
            }

            case PRINTF_STATE_LENGTH_SHORT:
            {
                if (*fmt == 'h')
                {
                    state = PRINTF_STATE_SPEC;
                    length = PRINTF_LENGTH_SHORT_SHORT;
                }
                else goto PRINTF_SPEC;
                break;
            }

            case PRINTF_STATE_SPEC:
            {
                PRINTF_SPEC:
                switch(*fmt)
                {
                    case 'c':
                        putc((char)va_arg(args,int));
                        break;
                    case 's':
                        puts(va_arg(args,char*));
                        break;
                    case 'x':
                    case 'X':
                    case 'p':
                        radix = 16; number = true; sign = false;
                        break;
                    case 'd':
                    case 'i':
                        radix = 10; number = true; sign = true;
                    case 'u':
                        radix = 10; number = true; sign = false;
                        break;
                    default:
                        putc(*fmt);
                        break;
                }
                if (number)
                {
                    if(!sign)
                    {
                        if (length == PRINTF_LENGTH_LONG)
                        {
                            printf_unsigned(va_arg(args,unsigned long),radix);
                        }
                        else if (length == PRINTF_LENGTH_LONG_LONG)
                        {
                            printf_unsigned(va_arg(args,unsigned long long),radix);
                        }
                        else if (length == PRINTF_LENGTH_SHORT)
                        {
                            printf_unsigned(va_arg(args,unsigned int),radix);
                        }
                        else if (length == PRINTF_LENGTH_SHORT_SHORT)
                        {
                            printf_unsigned(va_arg(args,unsigned int),radix);
                        }
                        else if (length == PRINTF_LENGTH_NORMAL)
                        {
                            printf_unsigned(va_arg(args,unsigned int),radix);
                        }
                    }
                    else
                    {
                        if (length == PRINTF_LENGTH_LONG)
                        {
                            printf_signed(va_arg(args,unsigned long),radix);
                        }
                        else if (length == PRINTF_LENGTH_LONG_LONG)
                        {
                            printf_signed(va_arg(args,unsigned long long),radix);
                        }
                        else if (length == PRINTF_LENGTH_SHORT)
                        {
                            printf_signed(va_arg(args,unsigned int),radix);
                        }
                        else if (length == PRINTF_LENGTH_SHORT_SHORT)
                        {
                            printf_signed(va_arg(args,unsigned int),radix);
                        }
                        else if (length == PRINTF_LENGTH_NORMAL)
                        {
                            printf_signed(va_arg(args,unsigned int),radix);
                        }
                    }
                }
                state = PRINTF_STATE_NORMAL;
                length = PRINTF_LENGTH_NORMAL;
                radix = 10;
                sign = false;
                number = false;
                break;
            }

            
        }
        fmt++;
    }
}