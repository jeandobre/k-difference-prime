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
     -vs1 (default): Matriz D[1 X n+1] (k1v1)
     -vs2: Matriz D[2 X n+1]           (k1v2)
     -vs3: Matriz D[m+1 X n+1]         (k1v2)

  -kdifferenceprime2: Abordagem 2
     -vs1 (default): Matriz L[1 X d+k+3] (k2v1)
     -vs2: Matriz L[d+k+3 X k+2]         (k2v2)

  -kdifferenceprime3: Abordagem 2 + Árvore de Sufixo Comprimida (CST)
     -vs1 (default): Matriz L[1 X d+k+3] (k3cst)

  -kdifferenceprime4: Abordagem 2 + Array de Sufixo (SA)
     -vs1 (default): Dmin                (k4dmin)
     -vs2 (default): RMQ                 (k4rmq)

  -kdifferenceprime5: Abordagem 2 + Árvore de Sufixo (ST)
     -vs1 (default): Matriz L[1 X d+k+3]  (k5st)

Dependências:
  kdifferenceprime1: classes.h
  kdifferenceprime2: classes.h
  kdifferenceprime3: classes.h, cst_v_1_1 ()
  kdifferenceprime4: classes.h, sa.h
  kdifferenceprime5: classes.h, strmat

Como executar os testes unitários?
   executar o test.sh no windows ou linux e selecionar a versão do programa conforme a lista acima;

Saída de dados:
   simples  = j ; tamanho ; sequencia
   completo = ocorrência; j ; tamanho ; sequencia ; j + r
   XML      = <ocorrencia><j><tamanho><sequencia>
   JSON     = {ocorrencia;j;tamanho;sequencia}

Parâmetros:

  -a sequência alfa: Arquivo ou sequência de caracteres
  -b sequência beta: Arquivo ou sequência de caracteres
  -k Parâmetro que indica a quantidade de diferenças desejada entre alfa e beta

  -vs[%] número da versão do programa de acordo com a lista de versões acima. (default = vs1)
  -sm Mostrar a matriz de programação dinâmica na tela (default = não mostrar)
  -st Mostrar o tempo de execução do algoritmo (default = não mostrar)
  -ts[%] tipo de saída de resultados. 1 = simples, 2 = completa, 3 = XML e 4 = JSON (default = 1)
  -sf nome do arquivo de saída: Nome da pasta e arquivo em que será salvo os resultados.
      (default = gerar automatico na pasta saida com o seguinte padrão de nome: aM_bN_k_nome_programa)
      exemplo: alpha: arquivo com 50 caracteres, beta: arquivo com 100 caracteres, k = 15, executado no KdifferencePrime1 com -vs1
               gerará o arquivo na pasta saida/a50_b100_k15_k1v1
  -log Mostrar log de etapas do processamento de dados
