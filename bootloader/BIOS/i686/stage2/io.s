.code32
.global i686_outb

i686_outb:
    movw 4(%esp),%dx
    movb 8(%esp),%al
    outb %al,%dx
    ret

.global i686_inb
i686_inb:
    movw 4(%esp),%dx
    xor %eax,%eax
    inb %dx,%al
    ret


