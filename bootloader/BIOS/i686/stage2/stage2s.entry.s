.code32

.global _start
.extern _cstart

_start:

StartC:
    call _cstart
    
#safety for if we get back here some how
#Which should be impossible
_loop:
    jmp _loop
