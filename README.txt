************************************************************************************
*
*
* Projeto de Trabalho de Mestrado em Ci�ncia da Computa��o - FACOM
* Aluno: Jean Alexandre Dobre
* Orientador: Prof. Dr. Said Sadique Adi
*
*
* Trabalho: O Problema da Sele��o de Primers Espec�ficos: algoritmos e aplica��es
*
*
*
************************************************************************************

Programas:

  -1: kdifferenceprime1.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11)
  -2: kdifferenceprime2.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11)
  -3: kdifferenceprime3.cpp (compilar: make -f makefilek3)
  -4: kdifferenceprime4.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11) 


Vers�es:
  -kdifferenceprime1: Abordagem 1 
     -vs1 (default): Matriz D[1 X n+1] 
     -vs2: Matriz D[2 X n+1]
     -vs3: Matriz D[m+1 X n+1] 

  -kdifferenceprime2: Abordagem 2
     -vs1 (default): Matriz L[1 X d+k+3]
     -vs2: Matriz L[d+k+3 X k+2]

  -kdifferenceprime3: Abordagem 2 + �rvore de Sufixo Comprimida (CST)
     -vs1 (default): Matriz L[1 X d+k+3]

  -kdifferenceprime4: Abordagem 2 + Array de Sufixo (SA)
     -vs1 (default): Matriz L[1 X d+k+3]
   
Depend�ncias:
  kdifferenceprime1: auxiliar.h, classes.h
  kdifferenceprime2: auxiliar.h, classes.h
  kdifferenceprime3: auxiliar.h, classes.h, cst_v_1_1 ()  
  kdifferenceprime4: auxiliar.h, classes.h, sa.h

Como executar os testes unit�rios?
   executar o test.sh no windows ou linux e selecionar a vers�o do programa conforme a lista acima;

Par�metros:

  -a
  -b
  -k

  -vs[%]
  -sm Mostrar a matriz de programa��o din�mica na tela
  -st Mostrar o tempo de execu��o do algoritmo