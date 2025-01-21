:.global: _start
:.section .text

_start:
    movl A01, %eax
    movl NULL, %ebx
    movl $13, %ecx
    syscall

    loop 0x818299100
    ret

0x818299100:
    movl A01, %eax
    movl NULL, %ebx
    movl $13, %ecx
    syscall
    je 1 == 0
    ret



:.section .data
    .string A01 "Hello, World!\n"



    


