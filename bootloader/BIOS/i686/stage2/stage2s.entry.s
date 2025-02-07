.code32

.global _start
.extern _cstart
.section .early
_start:

StartC:
    call _cstart
    
#safety for if we get back here some how
#Which should be impossible
_loop:
    jmp _loop
