### Exercise 1

**b) Suppose AMD wants to support virtual address spaces of 512 PB. What changes would need to be made to Figure 5.18? How many different "Page-Map Level-5" tables could then exist for a single process? Read sections 1.1.3 and 5.3.1 and indicate the ranges of canonical addresses for the proposed extension.**

----

**Changes to be made to Figure 5.18**
Para ter 512PB de memória de espaço vitual  é necessário, nos virtual address, criar outro offset para uma tabela level 6, Page-Map Level-6 Offset
(PML6). Para isso é necessário, apesar de haver 7 bits disponíveis, apenas se usa 2, deixando os restantes bits para sign extend.
512 PB = 2^12 * 2^9 * 2^9 * 2^9 * 2^9 * 2^9 * 2^2

**Maximum number of "Page-Map Level-5" tables per process**
Mínimo: 1 table    Máximo: 4 tables.
Page-Map Level-6 Table vai ter offset de 2 bits, entao apesar de ter 4kb, vai apenas poder ser acedido 4 bits. Logo vão ser de 1 a 4 tabelas Page-Map Level-5 Table.

**Ranges of canonical addresses**

2 ranges:

Ox0000 0000 0000 0000 .. Ox03FF FFFF FFFF FFFF
OxFC00 0000 0000 0000 .. OxFFFF FFFF FFFF FFFF

Intervalos de endereços canónicos a 64 bits, com 5 bits de sign extend igual ao 6º bit de maior peso, que é o bit de maior peso da nossa table Page-Map Level-6 Table.
