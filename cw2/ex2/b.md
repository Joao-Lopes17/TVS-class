        .section .rodata

data:   .byte  47, 101, 116,  99   # "/etc"
        .byte  47, 111, 115,  45   # "/os-"
        .byte 114, 101, 108, 101   # "rele"
        .byte  97, 115, 101,   0   # "ase\0"
        .byte  73,  83,  69,  76   # "ISEL"

        .text
        .globl _start
_start:
        mov x0, #-100           #x0 = rdi
        ldr x1, =data           #x1 = rsi
        mov x2, #0              #x2 = rdx
        mov w8, #56             # código do syscall com nome openat no ARM64
        svc #0

        mov x21, x0             #x21 = r15

        mov x2, #2
        mov x1, #0
        mov x0, x21
        mov w8, #62             # código do syscall com nome lseek no ARM64
        svc #0

        mov x20, x0             #x20 = r14

        mov x0, #0
        mov x5, #0              #x5 = r9
        mov x2, #1
        mov x3, #2              #x3 = r10
        mov x1, x20             
        mov x4, x21             #x4 = r8
        mov w8, #222            # código do syscall com nome mmap no ARM64
        svc #0

        mov x19, x0             #x19 = r13

        mov x1, x19
        mov x2, x20
        mov x0, #1
        mov w8, #64             # código do syscall com nome write no ARM64
        svc #0

        mov x0, #0
        mov w8, #94             # código do syscall com nome exit_group no ARM64
        svc #0

        .end

        x86_64          ARM64
->       rdi             x0
->       rsi             x1
->       rdx             x2
->       r10             x3
->       r8              x4
->       r9              x5
->       r15             x21
->       r14             x20
->       r13             x19
->       $257            #56
->       $8              #62
->       $9              #222
->       $1              #64
->       $231            #94

-> o valor de retorno do syscall, no ARM64, não é colocado no rax, e sim no x0 (equivalente ao rdi em X86_64).
