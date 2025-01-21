#define CCS_IMPL
#define LINUX
#include "CCs/CCS.h"


#include <string.h>

#include "buildsystem/BIOSBootloader.h"

int build = 0; // Debug or Release
int arch = 0; // i686 or x86_64
int boot = 0; // BIOS or UEFI

char* execution;
char* image_name = "build/ZSEOS.hdd"; //.hdd stands for hard drive disk

void Run_Qemu()
{
    //TODO: Make a accourding command depending on arch and boot
    if (boot == 0 && arch == 0 && build == 0)
    {
        CCS_CMD* runcmd = CCS_CreateCommand();
        CCS_SetCmdCommand(runcmd,"qemu-system-i386");
        CCS_AddArgument(runcmd,"-drive");
        char* driveinfo =(char*) malloc(strlen("id=disk,") + strlen("file=") + strlen(image_name)+strlen(",if=none") + 2);
        sprintf(driveinfo,"id=disk,file=%s,if=none",image_name);
        CCS_AddArgument(runcmd,driveinfo);
        CCS_AddArgument(runcmd,"-device ahci,id=ahci");
        CCS_AddArgument(runcmd,"-device ide-hd,drive=disk,bus=ahci.0");
        CCS_Execute_Command(runcmd,true);
        free(driveinfo);
        CCS_DestroyCommand(runcmd);
    }
}

int main(int argc,char* argv[])
{
    if (argc < 5)
    {
        // We put execution first so we can add stuff like help later
        printf("Build using: %s <execution> <Build> <arch> <boot>\n",argv[0]);
        return 1;
    }

    //Parsing the Build Type

    if (!strcmp(argv[2],"Debug") || !strcmp(argv[2],"debug") || !strcmp(argv[2],"dbg"))
    {
        build = 1;
    }
    else if (!strcmp(argv[2],"Release") || !strcmp(argv[2],"release") || !strcmp(argv[2],"rls"))
    {
        build = 0;
    }

    //Parsing the Architecture

    if (!strcmp(argv[3],"I686") || !strcmp(argv[3],"i686"))
    {
        arch = 0;
    }
    else if (!strcmp(argv[3],"x86_64") || !strcmp(argv[3],"amd64"))
    {
        arch = 1;
    }

    // Parsing the Boot method
    if (!strcmp(argv[4],"BIOS"))
    {
        boot = 0;
    }
    else if (!strcmp(argv[4],"UEFI"))
    {
        boot = 1;
    }

    if (!strcmp(argv[1],"run"))
    {
        Run_Qemu(image_name);
        return 1;
    }

    // This means the help command is not issued
    if (strcmp(argv[1],"help") && strcmp(argv[1],"clean"))
    {
        //Check for Build directory and then open file    
        FILE* image = NULL;
        if (!CCS_DoesFolderExist(NULL,"build") 
                && !CCS_DoesFolderExist(NULL,"build/OBJ") 
                && !CCS_DoesFolderExist(NULL,"build/OBJ/stage0")
                && !CCS_DoesFolderExist(NULL,"build/bin")
                ) // We can use NULL because the function does not use a command struct
        {
            // Issue a Create Build folder and open file
            CCS_CMD* mkbuild = CCS_CreateCommand();
            CCS_SetCmdCommand(mkbuild,"mkdir");
            CCS_AddArgument(mkbuild,"build");
            CCS_Execute_Command(mkbuild,true); // Annouce the command getting executed
            CCS_DestroyCommand(mkbuild);

            // make the command add OBJECT folder
            mkbuild = CCS_CreateCommand();
            CCS_SetCmdCommand(mkbuild,"mkdir");
            CCS_AddArgument(mkbuild,"build/OBJ");
            CCS_Execute_Command(mkbuild,true);
            CCS_DestroyCommand(mkbuild);

            // make the command add stage folder to the OBJECTS folder
            mkbuild = CCS_CreateCommand();
            CCS_SetCmdCommand(mkbuild,"mkdir");
            CCS_AddArgument(mkbuild,"build/OBJ/stage0");
            CCS_Execute_Command(mkbuild,true);

            CCS_DestroyCommand(mkbuild);
            // make the command add bin folder
            mkbuild = CCS_CreateCommand();
            CCS_SetCmdCommand(mkbuild,"mkdir");
            CCS_AddArgument(mkbuild,"build/bin");
            CCS_Execute_Command(mkbuild,true);

            CCS_DestroyCommand(mkbuild);
            //make the command add bin/bootloader folder
            mkbuild = CCS_CreateCommand();
            CCS_SetCmdCommand(mkbuild,"mkdir");
            CCS_AddArgument(mkbuild,"build/bin/bootloader");
            CCS_Execute_Command(mkbuild,true);

            CCS_DestroyCommand(mkbuild);

            CCS_CMD* ddfile = CCS_CreateCommand();
            CCS_SetCmdCommand(ddfile,"dd");
            CCS_AddArgument(ddfile,"if=/dev/zero");
            char* arg = (char*)malloc(strlen("of=") + strlen(image_name) + 3);
            strcpy(arg,"of=");
            strncat(arg,image_name,strlen(image_name));
            CCS_AddArgument(ddfile,arg);
            CCS_AddArgument(ddfile,"bs=512");
            CCS_AddArgument(ddfile,"count=2048");
            CCS_AddArgument(ddfile,">/dev/null");
            CCS_Execute_Command(ddfile,true);
            free(arg);
            CCS_DestroyCommand(ddfile);

            image = fopen(image_name,"rb+");
            if (image == NULL)
            {
                printf("Failed to open file\n");
                return 1;
            }

            goto BuildReady;
        }
        else 
        {
            BuildReady:
            mbrparttable[0].BootIndicator = 1;
            mbrparttable[0].OsType = ZSEOS_BOOT_DRIVE;
            mbrparttable[0].StartLBA = 2;
            mbrparttable[0].SizeLBA = 1024;
            uint8_t CHS[3] = {0};
            memcpy(mbrparttable[0].EndCHS,CHS,3);
            memcpy(mbrparttable[0].StartCHS,CHS,3);
            CCS_CMD* rmimage = CCS_CreateCommand();
            CCS_SetCmdCommand(rmimage,"rm");
            CCS_AddArgument(rmimage,"-rf");
            CCS_AddArgument(rmimage,image_name);
            CCS_Execute_Command(rmimage,true);
            CCS_DestroyCommand(rmimage);

            CCS_CMD* ddfile = CCS_CreateCommand();
            CCS_SetCmdCommand(ddfile,"dd");
            CCS_AddArgument(ddfile,"if=/dev/zero");
            char* arg = (char*)malloc(strlen("of=") + strlen(image_name) + 3);
            strcpy(arg,"of=");
            strncat(arg,image_name,strlen(image_name));
            CCS_AddArgument(ddfile,arg);
            CCS_AddArgument(ddfile,"bs=512");
            CCS_AddArgument(ddfile,"count=2048");
            CCS_AddArgument(ddfile,">/dev/null");
            CCS_Execute_Command(ddfile,true);
            free(arg);
            CCS_DestroyCommand(ddfile);

            //Open File
            image = fopen(image_name,"rb+");
            if (image == NULL)
            {
                printf("Failed to open file\n");
                return 1;
            }

        else if (!strcmp(argv[1],"build"))
        {
            //I686 and BIOS
            if (boot == 0 && arch == 0)
            {
                // Issue a assemble command
                CCS_CMD* assemble_stage0 = CCS_CreateCommand();
                CCS_SetCmdCommand(assemble_stage0,"i686-elf-as");  // Toolchain assembler for i686
                CCS_AddArgument(assemble_stage0,"-o build/OBJ/stage0/stage0.o");
                CCS_AddArgument(assemble_stage0,"-c bootloader/BIOS/stage0.s");

                CCS_Execute_Command(assemble_stage0,true);
                CCS_DestroyCommand(assemble_stage0);
                // Issue a link command
                CCS_CMD* link_stage0 = CCS_CreateCommand();
                CCS_SetCmdCommand(link_stage0,"i686-elf-ld");
                CCS_AddArgument(link_stage0,"-o build/bin/bootloader/stage0.bin");
                CCS_AddArgument(link_stage0,"build/OBJ/stage0/stage0.o");
                CCS_AddArgument(link_stage0,"-Ttext 0x7C00");
                CCS_AddArgument(link_stage0,"--oformat=binary");
                CCS_Execute_Command(link_stage0,true);
                CCS_DestroyCommand(link_stage0);

                if(!BIOS_WriteMBRToFile(image,"build/bin/bootloader/stage0.bin"))
                {
                    printf("Failed to write MBR\n");
                    return 1;
                }
            }
        }
        if (image == NULL)
        {
            printf("Image not here\n");
            return 0;
        }
        fclose(image);
        }
    }
    else if (!strcmp(argv[1],"clean"))
    {
        // issue a rm -rf build command
        CCS_CMD* rmbuild = CCS_CreateCommand();
        CCS_SetCmdCommand(rmbuild,"rm");
        CCS_AddArgument(rmbuild,"-rf");
        CCS_AddArgument(rmbuild,"build");
        CCS_Execute_Command(rmbuild,true);
        CCS_DestroyCommand(rmbuild);
    }


}