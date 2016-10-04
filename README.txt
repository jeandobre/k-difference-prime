************************************************************************************
*
* Projeto de Trabalho de Mestrado em Ciência da Computação - FACOM
* Aluno: Jean Alexandre Dobre
* Orientador: Prof. Dr. Said Sadique Adi
*
* Trabalho: O Problema da Seleção de Primers Específicos: algoritmos e aplicações
*
************************************************************************************

Programas:

  -1: kdifferenceprime1.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11)
  -2: kdifferenceprime2.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11)
  -3: kdifferenceprime3.cpp (compilar: make -f makefilek3)
  -4: kdifferenceprime4.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11) 
  -5: kdifferenceprime5.cpp (compilar: make -f makefilek5)


Versões:
  -kdifferenceprime1: Abordagem 1 
     -vs1 (default): Matriz D[1 X n+1] 
     -vs2: Matriz D[2 X n+1]
     -vs3: Matriz D[m+1 X n+1] 

  -kdifferenceprime2: Abordagem 2
     -vs1 (default): Matriz L[1 X d+k+3]
     -vs2: Matriz L[d+k+3 X k+2]

  -kdifferenceprime3: Abordagem 2 + Árvore de Sufixo Comprimida (CST)
     -vs1 (default): Matriz L[1 X d+k+3]

  -kdifferenceprime4: Abordagem 2 + Array de Sufixo (SA)
     -vs1 (default): Dmin
     -vs2 (default): RMQ
 
  -kdifferenceprime5: Abordagem 2 + Árvore de Sufixo (ST)
     -vs1 (default): Matriz L[1 X d+k+3]
   
Dependências:
  kdifferenceprime1: classes.h
  kdifferenceprime2: classes.h
  kdifferenceprime3: classes.h, cst_v_1_1 ()  
  kdifferenceprime4: classes.h, sa.h
  kdifferenceprime5: classes.h, strmat

Como executar os testes unitários?
   executar o test.sh no windows ou linux e selecionar a versão do programa conforme a lista acima;

Parâmetros:

  -a sequência alfa: Arquivo ou sequência de caracteres
  -b sequência beta: Arquivo ou sequência de caracteres
  -k Parâmetro que indica a quantidade de diferenças desejada entre alfa e beta

  -vs[%]
  -sm Mostrar a matriz de programação dinâmica na tela
  -st Mostrar o tempo de execução do algoritmo
