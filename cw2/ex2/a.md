### Exercise 2

**a) In the text file cw2/ex2/a.md, list and explain the sequence of calls performed by the program.**

----

.section .rodata
data:   .byte  47, 101, 116,  99   # "/etc"
        .byte  47, 111, 115,  45   # "/os-"
        .byte 114, 101, 108, 101   # "rele"
        .byte  97, 115, 101,   0   # "ase\0"
        .byte  73,  83,  69,  76   # "ISEL"

rax -> nome do syscall chamado

    movq $-100, %rdi        # rdi = -100 (AT_FDCWD, indica o diretório atual)
    leaq data(%rip), %rsi   # rsi = endereço da `data`, pointer para a string do path do ficheiro
    xorq %rdx, %rdx         # rdx = 0 (O_RDONLY, open for reading)
    movq $257, %rax         # rax = 257 (syscall nome `openat`)
    syscall                 # invoca o syscall
    
    movq %rax, %r15         #resultado do syscall (file descriptor) é guardado no r15

-> openat é usado para abrir um ficheiro. Neste caso o dirétorio é o atual (AT_FDCWD), com o path apontado pela data. Não se pode ler no file, visto ser apenas read-only(O_RDONLY).
-> Retorna o file descriptor do ficheiro.

    movq %r15, %rdi         # rdi = file descriptor (retorno de `openat`)
    xorq %rsi, %rsi         # rsi = 0 (deslocamento: a posição para onde ir no arquivo)
    movq $2, %rdx           # rdx = 2 (SEEK_END, move o ponteiro para o final do arquivo)
    movq $8, %rax           # rax = 8 (syscall nome `lseek`)
    syscall

    movq %rax, %r14         #resultado do syscall (tamanho do arquivo em bytes) é guardado no r14

-> lseek é usado para mover o ponteiro do arquivo. Neste caso, move o ponteiro para o final do arquivo (SEEK_END).
-> Retorna o tamanho do ficheiro.


    xorq %rdi, %rdi        # rdi = 0 (endereço de início: o kernel escolhe)
    movq %r14, %rsi        # rsi = tamanho do arquivo (retorno de `lseek`)
    movq $1, %rdx          # rdx = 1 (PROT_READ, proteção de leitura)
    movq $2, %r10          # r10 = MAP_PRIVATE (mapeamento privado -> alterações não afetão o arquivo)
    movq %r15, %r8         # r8 = descritor de arquivo (do `openat`)
    xorq %r9, %r9          # r9 = 0 (offset no arquivo: começar do início)
    movq $9, %rax          # rax = 9 (número do syscall `mmap`)
    syscall                

-> mmap é usado para mapear o conteúdo do arquivo na memória para leitura.
-> Retorna o endereço base da memória mapeada.

    movq $1, %rdi          # rdi = 1 (descritor para stdout)
    movq %rax, %rsi        # rsi = endereço base da memória mapeada (retorno do `mmap`)
    movq %r14, %rdx        # rdx = tamanho do arquivo (do `lseek`)
    movq $1, %rax          # rax = 1 (número do syscall `write`)
    syscall                

-> write é usado para escrever o conteúdo do arquivo (mapeado na memória) na saída padrão (stdout). 
-> Retorna a quantidade de bytes escritos.

    xorq %rdi, %rdi        # rdi = 0 (código de saída 0: sucesso)
    movq $231, %rax        # rax = 231 (número do syscall `exit_group`)
    syscall                

-> exit_group é usado para finalizar o programa. usa o código de saída 0, que é o código de sucesso

-> Este programa, com o uso de syscalls, abre o arquivo, determina o seu tamanho, mapeia o seu conteúdo na memória, exibe-o no terminal e encerra o programa com sucesso.

-> O programa aberto é bastante específico, sendo este o ficheiro que contêm informações do OS.
-> Path = /etc/os-release