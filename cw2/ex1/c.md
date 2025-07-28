### Exercise 1

**C) While processing the Present (P) bit in Page-Translation-Table Entry Fields (see Section 5.4.1), one of the entries in Table 8-1 is particularly relevant. Which entry is that, and why?**

----
Uma das entradas na Tabela 8-1 (**Interrupt Vector Source and Cause**) é particularmente relevante e está na posição 14, com a mnemônica #PF, denominada page-fault exception. Ocorre em acessos à memória quando a paginação está habilitada (bit P = 0), caso seja feita uma tentativa de acesso a uma tabela ou página.

Este bit P indica se a tabela de tradução de páginas ou a página física está carregada na memória física. 
Quando o bit P é definido como 1, a tabela ou página física é carregada na memória física, caso contrário não.

O software do sistema é responsável por carregar a tabela ou página ausente na memória e definir o bit P como 1.

Quando o bit P é 0, indicando uma página não presente, todos os bits restantes na entrada da estrutura de dados da página ficam disponíveis para o software.

As entradas com P a 0 nunca são armazenadas em cache no TLB nem o processador definirá o bit `Accessed` ou `Dirty` na entrada da tabela.