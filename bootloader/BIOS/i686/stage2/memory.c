#include "memory.h"

int memcmp(const void* ptr1,const void* ptr2,uint32_t size)
{
    const uint8_t* u8Ptr1 = (const uint8_t *)ptr1;
    const uint8_t* u8Ptr2 = (const uint8_t *)ptr2;

    for (uint16_t i = 0; i < size; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}