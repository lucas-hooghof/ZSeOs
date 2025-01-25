#TODO:
    #Get VESA frambuffer if existant
    #Load stage2
    #Jump to stage2

.code16
.global _start

_start:
    #Clear the screen
    movb $0,%ah
    movb $0x02,%al
    int $0x10
    #Load stage2
    call Load_Stage2
    #Detect Video mode
    call Detect_Video_Mode
    #Get memory map
    call Get_Memory_Map
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


#Will check for VESA
#If its not present it will set a value
Detect_Video_Mode:
    ret

#Get memory map
Get_Memory_Map:
    movl $0,%ebx

    xorw %ax,%ax
    movw %ax,%es

    movw $0x5000,%di
    movl $24,%ecx

    movl $0x534D4150,%edx

Loop:
    movl $0xE820,%eax
    int $0x15

    cmpl $0x534D4150,%eax
    jne End 
    cmpl $0,%ebx
    je End

    addw $24,%di

    jmp Loop
End:
    ret


Load_Stage2:
    movl $2,sector_start
    movw $DISK_DAP,%si
    movb $0x42,%ah
    int $0x13
    ret



DISK_DAP:
    .byte 0x10
    .byte 0x00
sector_count:
    .word 0x0001
offset:
    .word 0x8000
segment:
    .word 0x0000
sector_start:
    .quad 0x0000000000000000

#VESA FLAG
ISVESA_PRESENT: .byte 0

.code32
pmode_init:
    movl $0x10, %eax         # Load code segment selector for 32-bit code
    movl %eax, %ds           # Set DS
    movl %eax, %es           # Set ES
    movl %eax, %fs           # Set FS
    movl %eax, %gs           # Set GS
    movl $0x3000, %esp
    movl %esp,%ebp

    movl $0xB8000, %ebx

    movb $0x41,(%ebx)

    #push ISVESA_PRESENT
    jmp 0x8000

_loop32:
    jmp _loop32
