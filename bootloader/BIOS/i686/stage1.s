#TODO:
    #Get memory map
    #Get VESA frambuffer if existant
    #Load stage2
    #Jump to stage2

.code16
.global _start

_start:
    #Enable A20
    call EnableA20
    #Disable intterrupts
    cli
    #Load GDT
    lgdt gdtp
    #Set protected mode bit in cr0
    movl %cr0,%eax
    orl $1,%eax
    movl %eax,%cr0

    ljmp $0x08,$pmode_init
_loop:
    jmp _loop

EnableA20:
    #Check if supported
    movw $0x2403,%ax
    int $0x15
    jb A20IsDone
    cmpb $0,%ah
    jnz A20IsDone
    #Get A20 Status
    movw $0x2402,%ax              
    int $0x15   
    jb   A20IsDone             
    cmpb $0,%ah
    jnz  A20IsDone             

    cmpb  $1,%al
    jz  A20IsDone

    #Enable gate
    movw $0x2401,%ax                
    int $0x15
    jb A20IsDone              
    cmpb $0,%ah
    jnz     A20IsDone   
A20IsDone:
    ret
.align 16
gdtp:
    .word gdt_end - gdt_start - 1
    /* .long (0x07C0 << 4) + gdt */
    .long gdt_start

.align 16
gdt_start:
gdt_null:
    .quad 0
gdt_code_segment:
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b11001111
    .byte 0x00
gdt_data_segment:
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b11001111
    .byte 0x00
gdt_end:

.code32
pmode_init:
    movl $0x10, %eax         # Load code segment selector for 32-bit code
    movl %eax, %ds           # Set DS
    movl %eax, %es           # Set ES
    movl %eax, %fs           # Set FS
    movl %eax, %gs           #
    movl $0x3000, %esp

    movl $0xB8000, %ebx

    movb $0x41,(%ebx)

_loop32:
    jmp _loop32
