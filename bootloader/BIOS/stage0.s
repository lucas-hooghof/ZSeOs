.code16
.global _start

_start:
    cli #Turn of interrupt before starting to set segments

    movw $0,%ax
    movw %ax,%ds
    movw %ax,%es
    movw %ax,%fs
    movw %ax,%gs

    #Set stack to reasonable location
    movw $0x7C00,%bp
    movw %bp,%sp
    sti # turn interrupts back on

    #Store the boot drive
    movb %dl,BOOT_DRIVE

    #Check for disk extensions
    movb $0x41,%ah
    movb BOOT_DRIVE,%dl
    movw $0x55AA,%bx

    #calling the disk interrupt
    int $0x13

    #Check for any indecators of extensions not being present
    jc .UseCHS
    cmpw $0xAA55,%bx
    jne .UseCHS

.UseLBA:
    #Load partition 1 into register esi 
    lea part1,%esi
    #Load 0 into ax to count partitions
    movw $0,%ax

    partcheck:
        #check for bootindecator
        cmpb $1,(%esi)
        jne nextpart
        #check for ostype
        cmpb $0xCC,4(%esi)
        jne nextpart

        jmp foundpart
    nextpart:
        #Check if 4 is set in ax
        cmpw $4,%ax
        je FailedToFindBootPartition
        #Go to next partition
        addl $16,%esi
        incw %ax
        jmp partcheck

    foundpart:
        movl 8(%esi),%eax
        movl %eax,sector_start
        movw $1,sector_count
        movw $0,segment
        movw $0x7E00,offset
        movw $DISK_DAP,%si
        movb $0x42,%ah

        movb BOOT_DRIVE,%dl
        int $0x13

        jc FailedToReadFirstSectorOfBootpart

    jmp _loop

.UseCHS:
    movw $EXTENSIONS_NOT_PRESENT,%si
    call print
    jmp _loop

FailedToReadFirstSectorOfBootpart:

FailedToFindBootPartition:
    movw $BOOT_PARTITION_NOT_FOUND,%si
    call print
    jmp _loop

_loop:
    jmp _loop

print: 
    # Set the page number to 0
    xorb %bh,%bh
    # Set interrupt to 0x0E for write character to screen
    movb $0x0E,%ah

    # Load the character into al from value at si and increment si
    lodsb 

    #Check if the string has ended if so jump to end
    cmpb $0,%al
    je 1f

    # Call the interrupt
    int $0x10
    jmp print

1:
    ret

HELLO_WORLD: .asciz "Hello world!\n"
EXTENSIONS_NOT_PRESENT: .asciz "Extensions are not found. Reboot!\n402\n"
BOOT_PARTITION_NOT_FOUND: .asciz "Cant find boot partition!\n201\n"
BOOT_PART_NOT_READ: .asciz "Cant read boot partition. Reboot!\n202\n"
BOOT_DRIVE: .byte 0

DISK_DAP:
    .byte 0x10
    .byte 0x00
sector_count:
    .word 0x0000
offset:
    .word 0x0000
segment:
    .word 0x0000
sector_start:
    .quad 0x0000000000000000

.fill 440-(.-_start),1,0

.long 0
.word 0

part1: .fill 16,1,0
otherparts: .fill 48,1,0

.word 0xAA55
