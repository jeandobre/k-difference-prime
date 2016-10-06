************************************************************************************
*
* Projeto de Trabalho de Mestrado em Ci�ncia da Computa��o - FACOM
* Aluno: Jean Alexandre Dobre
* Orientador: Prof. Dr. Said Sadique Adi
*
* Trabalho: O Problema da Sele��o de Primers Espec�ficos: algoritmos e aplica��es
*
************************************************************************************

Programas:

  -1: kdifferenceprime1.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11)
  -2: kdifferenceprime2.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11)
  -3: kdifferenceprime3.cpp (compilar: make -f makefilek3)
  -4: kdifferenceprime4.cpp (compilar: g++ -O3 -Wfatal-errors -Wextra -Wall -std=c++11)
  -5: kdifferenceprime5.cpp (compilar: make -f makefilek5)


Vers�es:
  -kdifferenceprime1: Abordagem 1
     -vs1 (default): Matriz D[1 X n+1] (k1v1)
     -vs2: Matriz D[2 X n+1]           (k1v2)
     -vs3: Matriz D[m+1 X n+1]         (k1v2)

  -kdifferenceprime2: Abordagem 2
     -vs1 (default): Matriz L[1 X d+k+3] (k2v1)
     -vs2: Matriz L[d+k+3 X k+2]         (k2v2)

  -kdifferenceprime3: Abordagem 2 + �rvore de Sufixo Comprimida (CST)
     -vs1 (default): Matriz L[1 X d+k+3] (k3cst)

  -kdifferenceprime4: Abordagem 2 + Array de Sufixo (SA)
     -vs1 (default): Dmin                (k4dmin)
     -vs2 (default): RMQ                 (k4rmq)

  -kdifferenceprime5: Abordagem 2 + �rvore de Sufixo (ST)
     -vs1 (default): Matriz L[1 X d+k+3]  (k5st)

Depend�ncias:
  kdifferenceprime1: classes.h
  kdifferenceprime2: classes.h
  kdifferenceprime3: classes.h, cst_v_1_1 ()
  kdifferenceprime4: classes.h, sa.h
  kdifferenceprime5: classes.h, strmat

Como executar os testes unit�rios?
   executar o test.sh no windows ou linux e selecionar a vers�o do programa conforme a lista acima;

Sa�da de dados:
   simples  = j ; tamanho ; sequencia
   completo = ocorr�ncia; j ; tamanho ; sequencia ; j + r
   XML      = <ocorrencia><j><tamanho><sequencia>
   JSON     = {ocorrencia;j;tamanho;sequencia}

Par�metros:

  -a sequ�ncia alfa: Arquivo ou sequ�ncia de caracteres
  -b sequ�ncia beta: Arquivo ou sequ�ncia de caracteres
  -k Par�metro que indica a quantidade de diferen�as desejada entre alfa e beta

  -vs[%] n�mero da vers�o do programa de acordo com a lista de vers�es acima. (default = vs1)
  -sm Mostrar a matriz de programa��o din�mica na tela (default = n�o mostrar)
  -st Mostrar o tempo de execu��o do algoritmo (default = n�o mostrar)
  -ts[%] tipo de sa�da de resultados. 1 = simples, 2 = completa, 3 = XML e 4 = JSON (default = 1)
  -sf nome do arquivo de sa�da: Nome da pasta e arquivo em que ser� salvo os resultados.
      (default = gerar automatico na pasta saida com o seguinte padr�o de nome: aM_bN_k_nome_programa)
      exemplo: alpha: arquivo com 50 caracteres, beta: arquivo com 100 caracteres, k = 15, executado no KdifferencePrime1 com -vs1
               gerar� o arquivo na pasta saida/a50_b100_k15_k1v1
  -log Mostrar log de etapas do processamento de dados
