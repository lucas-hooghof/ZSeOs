#include "BIOSBootloader.h"

MBR_PARTITION mbrparttable[4];


bool BIOS_WriteMBRToFile(FILE* file,char* stage0code)
{
    MBR mbr = {
        .BootStrapCode = {0}, // To be set
        .UniqueMBRSig = 0, // Unused
        .unknown = 0,       // Unused
        .parttable = {0},
        .Signature = MBR_SIGNATURE
    };

    memcpy(mbr.parttable,mbrparttable,sizeof(mbrparttable));
    FILE* stage0 = fopen(stage0code,"rb");
    if (fread(mbr.BootStrapCode,1,MBR_BOOTSTRAP_CODE_SIZE,stage0) != MBR_BOOTSTRAP_CODE_SIZE)
    {
        printf("Failed to read bootstrapping code from stage0!\n");
        return false;
    }

    fwrite(&mbr,1,sizeof mbr,file);


    return true;
}